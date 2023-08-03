#ifndef CGROUPS_H
#define CGROUPS_H

// Used for cgroups limits initialization
#define CGROUP_MEMORY_MAX "1G"
#define CGROUP_MEMORY_OOM_MAX "1G"
#define CGROUP_CPU_WEIGHT "256"
#define CGROUP_PIDS_MAX "64"
enum { CGROUPS_FD_COUNT = 64, CGROUPS_CONTROL_FIELD_SIZE = 256 };

// Initializes cgroups for the hostname
int cgroups_init(char *hostname);
// Cleans up cgroups for the hostname
int cgroups_free(char *hostname);

#endif
