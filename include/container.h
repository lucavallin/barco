#ifndef CONTAINER_H
#define CONTAINER_H

#include <sys/types.h>

enum {
  // The stack size for the container
  CONTAINER_STACK_SIZE = (1024 * 1024),
};

// Represents the configuration for a container.
typedef struct {
  uid_t uid;
  int fd;
  char *hostname;
  char *cmd;
  char *arg;
  char *mnt;
} container_config;

// Initializes the container.
int container_init(container_config *config, char *stack);

// Waits for the container to exit.
int container_wait(int container_pid);

// Stops the container.
void container_stop(int container_pid);

#endif
