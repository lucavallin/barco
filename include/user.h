#ifndef USER_H
#define USER_H

#include <sys/types.h>

enum {
  // User namespace settings
  USER_NAMESPACE_OFFSET = 10000,
  USER_NAMESPACE_COUNT = 2000,
};

// Setup the user namespace for the process
int user_namespace_init(uid_t uid, int fd);

// Configures the user and group mappings for the namespace.
int user_namespace_set_user(pid_t pid, int fd);

#endif
