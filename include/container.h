#ifndef CONTAINER_H
#define CONTAINER_H

#include <errno.h>
#include <sys/types.h>

// Used to represent the result of a seccomp rule.
#define CONTAINER_SCMP_FAIL SCMP_ACT_ERRNO(EPERM)

enum {
  // The stack size for the container
  CONTAINER_STACK_SIZE = (1024 * 1024),
  // User namespace settings
  CONTAINER_USERNS_OFFSET = 10000,
  CONTAINER_USERNS_COUNT = 2000,
};

// Represents the configuration for a container.
typedef struct container_config {
  uid_t uid;
  int fd;
  char *hostname;
  const char **cmd;
  const char **mount_dir;
} container_config;

// Initializes the container.
int container_init(container_config *config, char *stack);

// Configures the container's user namespace
int container_update_map(pid_t container_pid, int *fd);

// Stops the container.
void container_stop(int container_pid);

// Destroys the container.
int container_wait(int container_pid);

#endif
