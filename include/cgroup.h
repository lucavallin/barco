#pragma once
#include "container.h"

// Initializes cgroup for the container
int cgroup_init(container_config *config);
// Cleans up cgroup for the container
int cgroup_free(container_config *config);
