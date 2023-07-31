#ifndef CGROUPS_H
#define CGROUPS_H

// Used for cgroups limits initialization
#define CGROUPS_MEMORY "1073741824"
#define CGROUPS_SHARES "256"
#define CGROUPS_PIDS "64"
#define CGROUPS_WEIGHT "10"
enum { CGROUPS_FD_COUNT = 64, CGROUPS_CONTROL_FIELD_SIZE = 256 };

// Initializes cgroups for the hostname
int cgroups_init(char *hostname);
// Cleans up cgroups for the hostname
int cgroups_free(char *hostname);

#endif
