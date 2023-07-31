#ifndef CONTAINER_H
#define CONTAINER_H

#include <errno.h>
#include <sys/types.h>

// Used to represent the result of a seccomp rule.
#define CONTAINER_SCMP_FAIL SCMP_ACT_ERRNO(EPERM)

// Represents the configuration for a container.
typedef struct container_config {
  uid_t uid;
  int fdr;
  char *hostname;
  const char **cmd;
  const char **mount_dir;
} container_config;

// Initializes the container.
int container_init(container_config *config, char *stack);

// Stops the container.
void container_stop(int container_pid);

// Destroys the container.
int container_destroy(int container_pid);

// Generates a random hostname for the container.
void container_hostname_generate(char *hostname);

#endif
