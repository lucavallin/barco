#ifndef USER_H
#define USER_H

#include <sys/types.h>

enum {
  // User namespace settings
  USER_NAMESPACE_UID_PARENT_RANGE_START = 0,
  USER_NAMESPACE_UID_CHILD_RANGE_START = 10000,
  USER_NAMESPACE_UID_CHILD_RANGE_SIZE = 2000,
};

// Setup the user namespace for the process
int user_namespace_init(uid_t uid, int fd);

// Configures the user and group mappings for the namespace
// so that the child process can set its own user and group
int user_namespace_prepare_mappings(pid_t pid, int fd);

#endif
