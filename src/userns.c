#define _GNU_SOURCE 1
#include "userns.h"
#include "../lib/log/log.h"
#include "container.h"
#include <fcntl.h>
#include <grp.h>
#include <linux/limits.h>
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int userns_set(container_config *config) {
  int has_userns = !unshare(CLONE_NEWUSER);
  int result = 0;

  log_debug("setting up user namespace...");
  log_debug("writing to container socket...");
  if (write(config->fd, &has_userns, sizeof(has_userns)) !=
      sizeof(has_userns)) {
    log_error("write to socket failed: %m");
    return -1;
  }

  log_debug("reading from container socket...");
  if (read(config->fd, &result, sizeof(result)) != sizeof(result)) {
    log_error("read from socket failed: %m");
    return -1;
  }

  if (result) {
    return -1;
  }

  if (has_userns) {
    log_debug("user namespace setup complete");
  } else {
    log_debug("user namespaces probably unsupported, continuing...");
  }

  log_debug("switching to uid %d / gid %d...", config->uid, config->uid);

  log_debug("setting uid and gid mappings...");
  if (setgroups(1, &(gid_t){config->uid}) ||
      setresgid(config->uid, config->uid, config->uid) ||
      setresuid(config->uid, config->uid, config->uid)) {
    log_error("setting uid and gid mappings failed: %m");
    return -1;
  }

  log_debug("user namespace setup complete");

  return 0;
}

// Listens for the container to request setting uid and gid mappings.
// If successful, isetgroups, setresgid, and setresuid are called.
// setgroups and setresgid are necessary because of two separate group
// mechanisms on Linux. The function assumes that every uid has a corresponding
// gid, which is often the case.
int userns_set_user(pid_t container_pid, int fd) {
  int uid_map = 0;
  int has_userns = -1;

  log_debug("updating container map...");
  log_debug("retrieving user namespaces status...");
  if (read(fd, &has_userns, sizeof(has_userns)) != sizeof(has_userns)) {
    log_error("retrieve status failed: %m");
    return -1;
  }

  if (has_userns) {
    char path[PATH_MAX] = {0};

    log_debug("user namespaces enabled");

    log_debug("writing uid_map and gid_map...");
    for (char **file = (char *[]){"uid_map", "gid_map", 0}; *file; file++) {
      if (snprintf(path, sizeof(path), "/proc/%d/%s", container_pid, *file) >
          (int)sizeof(path)) {
        log_error("path setup failed: %m");
        return -1;
      }

      log_debug("writing %s...", path);
      if ((uid_map = open(path, O_WRONLY)) == -1) {
        log_error("open failed: %m");
        return -1;
      }

      log_debug("writing settings...");
      if (dprintf(uid_map, "0 %d %d\n", USERNS_OFFSET, USERNS_COUNT) == -1) {
        log_error("writing to uid_map failed: %m");
        close(uid_map);
        return -1;
      }
      close(uid_map);
    }

    log_debug("uid_map and gid_map updated");
  }

  log_debug("updating socket");
  if (write(fd, &(int){0}, sizeof(int)) != sizeof(int)) {
    log_error("socket update failed: %m");
    return -1;
  }

  return 0;
}
