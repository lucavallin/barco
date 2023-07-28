#pragma once
#include <sys/types.h>

// Represents the configuration for a container.
typedef struct {
  int argc;
  uid_t uid;
  int fd;
  char *hostname;
  char **argv;
  char *mount_dir;
} container_config;

// Initializes the container.
int container_init(container_config *config, char *stack);

// Stops the container.
void container_stop(int container_pid);

// Destroys the container.
int container_destroy(int container_pid);

// Listens for the container to request setting uid and gid mappings.
// If successful, isetgroups, setresgid, and setresuid are called.
// setgroups and setresgid are necessary because of two separate group
// mechanisms on Linux.
int container_set_userns(container_config *config);
