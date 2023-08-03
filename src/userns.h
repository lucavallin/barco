#ifndef USERNS_H
#define USERNS_H

#include "container.h"
#include <sys/types.h>

enum {
  // User namespace settings
  USERNS_OFFSET = 10000,
  USERNS_COUNT = 2000,
};

// Setup the user namespace for the container
int userns_set(container_config *config);

// Configures the user and group mappings for the namespace.
int userns_set_user(pid_t container_pid, int fd);

#endif
