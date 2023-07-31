#include "../include/cgroup.h"
#include "../include/container.h"
#include "../include/namespace.h"
#include "../lib/argtable/argtable3.h"
#include "../lib/log.c/src/log.h"
#include <stdio.h>
#include <stdlib.h>

enum {
  // BASE_10 is the base of the number system,
  // used to convert a string to a number.
  BASE_10 = 10,
  // HOSTNAME_LEN is the length of the hostname
  HOSTNAME_LEN = 32,
  // ARGTABLE_ARG_MAX is the maximum number of arguments
  ARGTABLE_ARG_MAX = 20
};

/* global arg_xxx structs */
struct arg_lit *help, *version;
struct arg_int *uid;
struct arg_str *img;
struct arg_str *cmd;
struct arg_end *end;

int main(int argc, char **argv) {
  // used for container stack
  char *stack = 0;
  // used for container hostname
  char hostname[HOSTNAME_LEN];
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
      uid = arg_intn("u", "uid", "<n>", 1, 1, "set the uid of the container"),
      cmd = arg_strn("c", "cmd", "<s>", 1, 1,
                     "set the command to run in the container"),
      img = arg_strn("m", "img", "<s>", 1, 1,
                     "set the mount path to use for the container"),
      end = arg_end(ARGTABLE_ARG_MAX),
  };

  int exitcode = 0;
  char progname[] = "barco";

  int nerrors;
  nerrors = arg_parse(argc, argv, argtable);

  /* special case: '--help' takes precedence over error reporting */
  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    printf("Demonstrate command-line parsing in argtable3.\n\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    exitcode = 0;
    goto exit;
  }

  /* If the parser returned any errors then display them and exit */
  if (nerrors > 0) {
    /* Display the error details contained in the arg_end struct.*/
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }

  config.cmd = cmd->sval;
  config.mount_dir = img->sval;

  // Set hostname for the container to "barcontainer"
  config.hostname = "barcontainer";

  // Initialize a socket pair to communicate with the container
  if (namespace_socket_pair_init(sockets) != 0) {
    log_error("namespace_socket_pair_init failed");
    exitcode = 1;
    goto exit;
  }
  config.fdr = sockets[1];

  // Initialize a stack for the container
  if (namespace_stack_init(&stack) != 0) {
    log_error("namespace_stack_init failed");
    exitcode = 1;
    goto cleanup;
  }

  // Prepare cgroup for the process tree (the container is a child of the
  // barco process)
  if (cgroup_init(&config)) {
    exitcode = 1;
    goto cleanup;
  }

  // Initialize the container (calls clone() internally)
  // Stacks on most architectures grow downwards.
  // NAMESPACE_STACK_SIZE gives us a pointer just below the end.
  container_pid = container_init(&config, stack + NAMESPACE_STACK_SIZE);
  if (container_pid == -1) {
    log_error("container_init failed");
    exitcode = 1;
    goto cleanup;
  }

  // Configures the container's user namespace and
  // pause until its process tree exits
  if (namespace_set_uid_map(container_pid, sockets[0])) {
    exitcode = 1;
    goto stop_and_destroy_container;
  }
  goto destroy_container;

stop_and_destroy_container:
  if (container_pid) {
    container_stop(container_pid);
  }

destroy_container:
  exitcode |= container_destroy(container_pid);

cleanup:
  // Clear resources (cgroup, stack, sockets)
  cgroup_free(&config);
  free(stack);
  namespace_socket_pair_close(sockets);

exit:
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
  return exitcode;
}
