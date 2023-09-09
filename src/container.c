#define _GNU_SOURCE 1
#include "container.h"
#include "log.h"
#include "mount.h"
#include "sec.h"
#include "user.h"
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

char *join_strings(const char *strings[], int start, int end);

// This is the function that will be called by clone() to start the container.
// The order of the operations is of important as, for example,
// mounts cannot be changed without specific capabilities,
// unshare cannot be called after syscalls are limited, etc...
int container_start(void *arg) {
  container_config *config = arg;

  log_debug("starting container");
  log_debug(
      "setting hostname, mounts, user namespace, capabilities and syscalls...");

  if (sethostname(config->hostname, strlen(config->hostname)) ||
      mount_set(config->mnt) || user_namespace_init(config->uid, config->fd) ||
      sec_set_caps() || sec_set_seccomp()) {
    log_debug("failed to set properties");
    close(config->fd);
    return -1;
  }

  log_debug("closing container socket...");
  if (close(config->fd)) {
    log_error("failed to close container socket: %m");
    return -1;
  }

  int exit = 0;

  char **argv = malloc((config->arg_length + 2 * sizeof(char *)));
  if (argv == NULL) {
    log_error("failed to allocate memory for argv: %m");
    return -1;
  }
  int argv_print_length = 0;
  argv[0] = config->cmd;
  for (int i = 0; i < config->arg_length; i++) {
    argv[i + 1] = config->arg[i];
    argv_print_length += strlen(config->arg[i]) + 1;
  }
  // argv must be NULL terminated
  argv[config->arg_length + 1] = NULL;
  char *argv_echo_string = join_strings(argv, 1, config->arg_length);

  log_debug("executing command '%s %s' from directory '%s' in container...",
            config->cmd, argv_echo_string, config->mnt);
  log_info("### BARCONTAINER STARTING - type 'exit' to quit ###");

  if (execve(config->cmd, argv, NULL)) {
    log_error("failed to execve '%s%s%s': %m", config->cmd,
              config->arg == NULL ? "" : " ",
              config->arg == NULL ? "" : argv_print_length);
    exit = -1;
    goto cleanup;
  }
  log_debug("container started...");

cleanup:
  free(argv);
  free(argv_echo_string);

  if (exit == -1) {
    return -1;
  }

  return 0;
}

// Creates container (process) with different properties than its parent
// e.g. mount to different dir, different hostname, etc...
// All these requirements are specified by the flags we pass to clone()
int container_init(container_config *config, char *stack) {
  int container_pid = 0;
  // The flags specify what the cloned process can do.
  // These allow some control overrmounts, pids, IPC data structures, network
  // devices and hostname.
  int flags = CLONE_NEWNS | CLONE_NEWCGROUP | CLONE_NEWPID | CLONE_NEWIPC |
              CLONE_NEWNET | CLONE_NEWUTS;

  // SIGCHLD lets us wait on the child process.
  log_debug("cloning process...");
  if ((container_pid =
           clone(container_start, stack, flags | SIGCHLD, config)) == -1) {
    log_error("failed to clone: %m");
    return 1;
  }

  // Close and zero the child's socket, to avoid leaving an open fd in
  // case of a failure. If we do not do this, the child or parent might hang.
  log_debug("resetting container socket...");
  close(config->fd);
  config->fd = 0;

  return container_pid;
}

int container_wait(int container_pid) {
  int container_status = 0;

  log_debug("waiting for container_pid %d...", container_pid);
  waitpid(container_pid, &container_status, 0);
  log_debug("container_pid %d exited", container_pid);

  return WEXITSTATUS(container_status);
}

void container_stop(int container_pid) {
  log_debug("calling kill for container_pid %d...", container_pid);
  if (kill(container_pid, SIGKILL)) {
    log_error("failed to kill container_pid %d: %m", container_pid);
  }
  log_debug("container_pid %d killed", container_pid);
}

char *join_strings(const char *strings[], int start, int end) {
  char *separator = " ";
  if (end <= start) {
    return NULL;
  }
  int totalLength = 0;
  for (int i = start; i <= end; i++) {
    totalLength += strlen(strings[i]);
  }
  totalLength += (end - start - 1) * strlen(separator) + 1;

  char *result = (char *)malloc(totalLength);
  if (result == NULL) {
    return NULL;
  }

  result[0] = '\0'; // Ensure the string is initially empty

  for (int i = start; i <= end; i++) {
    strcat(result, strings[i]);
    if (i != end) {
      strcat(result, separator);
    }
  }

  return result;
}
