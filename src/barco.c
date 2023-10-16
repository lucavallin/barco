#include "argtable3.h"
#include "cgroups.h"
#include "container.h"
#include "log.h"
#include "user.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

enum {
  // ARGTABLE_ARG_MAX is the maximum number of arguments
  ARGTABLE_ARG_MAX = 20
};

/* global arg_xxx structs */
struct arg_lit *help, *version;
struct arg_int *uid;
struct arg_str *mnt;
struct arg_str *cmd;
struct arg_str *arg;
struct arg_lit *vrb;
struct arg_end *end;

int main(int argc, char **argv) {
  // used for container stack
  char *stack = 0;
  // used for container config
  container_config config = {0};
  // used for container pid
  int container_pid = 0;
  // socket pair used for communication between barco and container
  int sockets[2] = {0};

  // the global arg_xxx structs are initialised within the argtable
  void *argtable[] = {
      help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
      version =
          arg_litn(NULL, "version", 0, 1, "display version info and exit"),
      uid = arg_intn("u", "uid", "<n>", 1, 1,
                     "uid and gid of the user in the container"),
      mnt = arg_strn("m", "mnt", "<s>", 1, 1,
                     "directory to mount as root in the container"),
      cmd =
          arg_strn("c", "cmd", "<s>", 1, 1, "command to run in the container"),
      arg =
          arg_strn("a", "arg", "<s>", 0, 1, "argument to pass to the command"),
      vrb = arg_litn("v", "verbosity", 0, 1, "verbose output"),
      end = arg_end(ARGTABLE_ARG_MAX),
  };

  int exitcode = 0;
  char progname[] = "barco";

  int nerrors;
  nerrors = arg_parse(argc, argv, argtable);

  // special case: '--help' takes precedence over error reporting
  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    exitcode = 0;
    goto exit;
  }

  // If the parser returned any errors then display them and exit
  if (nerrors > 0) {
    // Display the error details contained in the arg_end struct.
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }

  // Set verbosity level
  log_set_level(LOG_INFO);
  if (vrb->count > 0) {
    log_set_level(LOG_TRACE);
  }

  config.cmd = (char *)cmd->sval[0];
  config.arg = (char *)arg->sval[0];
  config.mnt = (char *)mnt->sval[0];

  // Check if barco is running as root
  if (geteuid() != 0) {
    log_warn("barco should be run as root");
  }

  // Set hostname for the container to "barcontainer"
  config.hostname = "barcontainer";

  // Initialize a socket pair to communicate with the container
  log_info("initializing socket pair...");
  if (socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, sockets)) {
    log_fatal("failed to initialialize socket pair: %m");
    exitcode = 1;
    goto exit;
  }

  log_info("setting socket flags...");
  if (fcntl(sockets[0], F_SETFD, FD_CLOEXEC)) {
    log_fatal("failed to socket fcntl: %m");
    exitcode = 1;
    goto exit;
  }
  config.fd = sockets[1];

  // Initialize a stack for the container
  log_info("initializing container stack...");
  if (!(stack = malloc(CONTAINER_STACK_SIZE))) {
    log_fatal("failed to initialize container stack: %m");
    exitcode = 1;
    goto cleanup;
  }

  // Initialize the container (calls clone() internally).
  log_info("initializing container...");
  // Stacks on most architectures grow downwards.
  // CONTAINER_STACK_SIZE gives us a pointer just below the end.
  if ((container_pid = container_init(&config, stack + CONTAINER_STACK_SIZE)) ==
      -1) {
    log_fatal("failed to container_init");
    exitcode = 1;
    goto cleanup;
  }

  // Prepare cgroups for the process (the container is a child process of barco)
  log_info("initializing cgroups...");
  if (cgroups_init(config.hostname, container_pid)) {
    log_fatal("failed to initialize cgroups");
    exitcode = 1;
    goto cleanup;
  }

  // Barco configures the user namespace for the container
  log_info("configuring user namespace...");
  if (user_namespace_prepare_mappings(container_pid, sockets[0])) {
    exitcode = 1;
    log_fatal("failed to user_namespace_set_user, stopping container...");
    goto cleanup;
  }

  // Wait for the container to exit
  log_info("waiting for container to exit...");
  exitcode |= container_wait(container_pid);
  log_debug("container exited...");

cleanup:
  // Clear resources (cgroups, stack, sockets)
  log_info("freeing resources...");

  log_debug("freeing stack...");
  free(stack);

  log_debug("freeing sockets...");
  close(sockets[0]);
  close(sockets[1]);

  log_debug("freeing cgroups...");
  cgroups_free(config.hostname);

exit:
  log_debug("freeing argtable...");
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
  log_info("so long and thanks for all the fish");
  return exitcode;
}
