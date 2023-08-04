#ifndef CGROUPS_H
#define CGROUPS_H

#include <fcntl.h>

// Used for cgroups limits initialization
#define CGROUPS_MEMORY_MAX "1G"
#define CGROUPS_CPU_WEIGHT "256"
#define CGROUPS_PIDS_MAX "64"
#define CGROUPS_CGROUP_PROCS "cgroup.procs"
enum { CGROUPS_CONTROL_FIELD_SIZE = 256 };

// Initializes cgroups for the hostname
int cgroups_init(char *hostname, pid_t pid);
// Cleans up cgroups for the hostname
int cgroups_free(char *hostname);

#endif
