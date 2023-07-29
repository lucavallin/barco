#include "../include/cgroup.h"
#include "../include/container.h"
#include "../include/hostname.h"
#include "../include/namespace.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// BASE_10 is the base of the number system,
// used to convert a string to a number.
#define BASE_10 10

int main(int argc, char **argv) {
  // used for strto* functions
  char *endptr;
  // used for container stack
  char *stack = 0;
  // used for container hostname
  char hostname[HOSTNAME_LEN];
  // used for container config
  container_config config = {0};
  // used for container pid
  int container_pid = 0;
  // return status of barco
  int err = 0;
  // used for parsing arguments to barco
  int last_optind = 0;
  // used for parsing arguments to barco
  int option = 0;
  // socket pair used for communication between barco and container
  int sockets[2] = {0};
  // used for parsing arguments to barco
  long int result = 0;

  // Parse arguments
  while ((option = getopt(argc, argv, "c:m:u:"))) {
    switch (option) {
    case 'c':
      config.argc = argc - last_optind - 1;
      config.argv = &argv[argc - config.argc];
      goto finish_options;

    case 'm':
      config.mount_dir = optarg;
      break;

    case 'u':
      result = strtol(optarg, &endptr, BASE_10);

      // Check for errors in conversion
      if (*endptr != '\0' || endptr == optarg) {
        fprintf(stderr, "malformed uid: %s\n", optarg);
        goto usage;
      } else {
        config.uid = (int)result;
      }
      break;

    default:
      goto usage;
    }

    last_optind = optind;
  }

  // Check that we have all the arguments we need
finish_options:
  if (!config.argc || !config.mount_dir) {
    goto usage;
  }

  // Here we could check that the Linux version is between 4.7.x and 4.8.x on
  // x86_64 with e.g. version_check() in src/version.c. Since we might want to
  // block system calls and capabilities, we need to make sure there aren't new
  // ones.

  // Set hostname for the container to a random string
  hostname_generate(hostname);
  config.hostname = hostname;

  // Initialize a socket pair to communicate with the container
  if (namespace_socket_pair_init(sockets) != 0) {
    fprintf(stderr, "=> namespace_socket_pair_init failed\n");
    goto error;
  }
  config.fd = sockets[1];

  // Initialize a stack for the container
  if (namespace_stack_init(&stack) != 0) {
    fprintf(stderr, "=> namespace_stack_init failed\n");
    goto error;
  }

  // Prepare cgroup for the process tree (the container is a child of the
  // barco process)
  if (cgroup_init(&config)) {
    err = 1;
    goto cleanup;
  }

  // Initialize the container (calls clone() internally)
  // Stacks on most architectures grow downwards.
  // NAMESPACE_STACK_SIZE gives us a pointer just below the end.
  container_pid = container_init(&config, stack + NAMESPACE_STACK_SIZE);
  if (container_pid == -1) {
    fprintf(stderr, "=> container_init failed\n");
    err = 1;
    goto cleanup;
  }

  // Configures the container's user namespace and
  // pause until its process tree exits
  if (namespace_handle_container_uid_map(container_pid, sockets[0])) {
    err = 1;
    goto stop_and_destroy_container;
  }

  goto destroy_container;

  // Stop and destroy the container
stop_and_destroy_container:
  if (container_pid) {
    container_stop(container_pid);
  }

  // Destroy the container
destroy_container:
  err |= container_destroy(container_pid);

  // Clear resources (cgroup, stack, sockets)
cleanup:
  cgroup_free(&config);
  free(stack);
  namespace_socket_pair_close(sockets);
  goto exit;

  // Print usage information
usage:
  fprintf(stderr, "Usage: %s -u -1 -m . -c /bin/sh ~\n", argv[0]);

  // Set error status
error:
  err = 1;

  // Exit program
exit:
  return err;
}
