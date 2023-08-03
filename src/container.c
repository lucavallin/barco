#define _GNU_SOURCE 1
#include "container.h"
#include "../lib/log/log.h"
#include "mount.h"
#include "security.h"
#include "userns.h"
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// The order of the operations is of important as, for example,
// mounts cannot be changed without specific capabilities,
// unshare cannot be called after syscalls are limited, etc...
int container_start(void *arg) {
  container_config *config = arg;

  log_debug("starting container");
  log_debug(
      "setting hostname, mounts, user namespace, capabilities and syscalls...");

  if (sethostname(config->hostname, strlen(config->hostname)) ||
      mount_set(config) || userns_set(config) || security_set_caps() ||
      security_set_seccomp()) {
    log_debug("failed to set properties");
    close(config->fd);
    return -1;
  }

  log_debug("closing container socket...");
  if (close(config->fd)) {
    log_error("socket closings failed: %m");
    return -1;
  }

  log_debug("executing command '%s %s' from directory '%s' in container...",
            config->cmd, config->arg, config->mnt);
  log_info("### BARCONTAINER STARTING - type 'exit' to quit ###");
  // argv must be NULL terminated
  char *argv[] = {config->arg, NULL};
  if (execve(config->cmd, argv, NULL)) {
    log_error("execve failed: %m");
    return -1;
  }
  log_debug("container started...");

  return 0;
}

// Creates container (process) with different properties than its parent
// e.g. mount to different dir, different hostname, etc...
// All these requirements are specified by the flags we pass to clone()
int container_init(container_config *config, char *stack) {
  int container_pid = 0;
  // The flags namespace the mounts, pids, IPC data structures, network devices
  // and hostname.
  int flags = CLONE_NEWNS | CLONE_NEWCGROUP | CLONE_NEWPID | CLONE_NEWIPC |
              CLONE_NEWNET | CLONE_NEWUTS;

  // SIGCHLD lets us wait on the child process.
  log_debug("cloning process...");
  if ((container_pid =
           clone(container_start, stack, flags | SIGCHLD, config)) == -1) {
    log_error("clone failed: %m");
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
  kill(container_pid, SIGKILL);
  log_debug("container_pid %d killed", container_pid);
}
