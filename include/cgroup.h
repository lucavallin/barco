#ifndef CGROUP_H
#define CGROUP_H

#include "container.h"

// Used for cgroup limits initialization
#define CGROUP_MEMORY "1073741824"
#define CGROUP_SHARES "256"
#define CGROUP_PIDS "64"
#define CGROUP_WEIGHT "10"
enum { CGROUP_FD_COUNT = 64, CGROUP_CONTROL_FIELD_SIZE = 256 };

// Initializes cgroup for the container
int cgroup_init(container_config *config);
// Cleans up cgroup for the container
int cgroup_free(container_config *config);

#endif
