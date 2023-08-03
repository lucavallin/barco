#define _GNU_SOURCE 1
#include "user.h"
#include "log.h"
#include <fcntl.h>
#include <grp.h>
#include <linux/limits.h>
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Listens for the process to request setting uid and gid mappings.
// If successful, isetgroups, setresgid, and setresuid are called.
// setgroups and setresgid are necessary because of two separate group
// mechanisms on Linux. The function assumes that every uid has a corresponding
// gid, which is often the case.
int user_namespace_init(uid_t uid, int fd) {
  int userns_enabled = !unshare(CLONE_NEWUSER);
  int result = 0;

  log_debug("setting user namespace...");
  log_debug("writing to socket...");
  if (write(fd, &userns_enabled, sizeof(userns_enabled)) !=
      sizeof(userns_enabled)) {
    log_error("failed to write socket %d: %m", fd);
    return -1;
  }

  log_debug("reading from socket...");
  if (read(fd, &result, sizeof(result)) != sizeof(result)) {
    log_error("failed to read from socket %d: %m", fd);
    return -1;
  }

  if (result) {
    return -1;
  }

  if (userns_enabled) {
    log_debug("user namespaces are enabled...");
  } else {
    log_debug("user namespaces unsupported, continuing...");
  }

  log_debug("switching to uid %d / gid %d...", uid, uid);

  log_debug("setting uid and gid mappings...");
  if (setgroups(1, &(gid_t){uid}) || setresgid(uid, uid, uid) ||
      setresuid(uid, uid, uid)) {
    log_error("failed to set uid %d / gid %d mappings: %m", uid, uid);
    return -1;
  }

  log_debug("user namespace set");

  return 0;
}

// Listens for the process to request setting uid and gid mappings.
// If successful, isetgroups, setresgid, and setresuid are called.
// setgroups and setresgid are necessary because of two separate group
// mechanisms on Linux. The function assumes that every uid has a corresponding
// gid, which is often the case.
int user_namespace_set_user(pid_t pid, int fd) {
  int uid_map = 0;
  int userns_enabled = -1;

  log_debug("updating uid_map / gid_map...");
  log_debug("retrieving user namespaces status...");
  if (read(fd, &userns_enabled, sizeof(userns_enabled)) !=
      sizeof(userns_enabled)) {
    log_error("failed to retrieve status from socket %d: %m", fd);
    return -1;
  }

  if (!userns_enabled) {
    log_debug("user namespaces not enabled");
  } else {
    char path[PATH_MAX] = {0};

    log_debug("user namespaces enabled");

    log_debug("writing uid_map / gid_map...");
    for (char **file = (char *[]){"uid_map", "gid_map", 0}; *file; file++) {
      if (snprintf(path, sizeof(path), "/proc/%d/%s", pid, *file) >
          (int)sizeof(path)) {
        log_error("failed to setup path %s: %m", path);
        return -1;
      }

      log_debug("writing %s...", path);
      if ((uid_map = open(path, O_WRONLY)) == -1) {
        log_error("failed to open %s: %m", path);
        return -1;
      }

      log_debug("writing settings...");
      if (dprintf(uid_map, "0 %d %d\n", USER_NAMESPACE_OFFSET,
                  USER_NAMESPACE_COUNT) == -1) {
        log_error("failed to write uid_map '%d': %m", uid_map);
        close(uid_map);
        return -1;
      }
      close(uid_map);
    }

    log_debug("uid_map and gid_map updated");
  }

  log_debug("updating socket");
  if (write(fd, &(int){0}, sizeof(int)) != sizeof(int)) {
    log_error("failed to update socket %d: %m", fd);
    return -1;
  }

  return 0;
}
