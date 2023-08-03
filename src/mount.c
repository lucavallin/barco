#include "mount.h"
#include "../lib/log/log.h"
#include "container.h"
#include <bsd/string.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>

// pivot_root is a system call to swap the mount at / with another.
// glibc does not provide a wrapper for it.
long pivot_root(const char *new_root, const char *put_old) {
  log_debug("calling pivot_root syscall...");
  return syscall(SYS_pivot_root, new_root, put_old);
}

// Restricts access to resources the container has in its own mount namespace:
// - Create a temporary directory and one inside of it
// - Bind mount of the user argument onto the temporary directory
// - pivot_root makes the bind mount the new root and mounts the old root onto
// the inner temporary directory
// - umount the old root and remove the inner temporary directory.
//
// Notice: The container is not being packed/unpacked. This
// is risky if the mounted directory contains sensitive data.
int mount_set(container_config *config) {
  log_debug("setting up mounts...");

  // MS_PRIVATE makes the bind mount invisible outside of the namespace
  // MS_REC makes the mount recursive
  log_debug("remounting with MS_PRIVATE...");
  if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL)) {
    log_error("failed to mount: %m");
    return -1;
  }
  log_debug("remounted");

  log_debug("creating temporary directory and...");
  char mount_dir[] = "/tmp/barco.XXXXXX";
  if (!mkdtemp(mount_dir)) {
    log_error("directory creation failed: %m");
    return -1;
  }

  log_debug("bind mount...");
  if (mount(config->mnt, mount_dir, NULL, MS_BIND | MS_PRIVATE, NULL)) {
    log_error("bind mount failed for path %s: %m", config->mnt);
    return -1;
  }

  log_debug("creating inner directory...");
  char inner_mount_dir[] = "/tmp/barco.XXXXXX/oldroot.XXXXXX";
  memcpy(inner_mount_dir, mount_dir, sizeof(mount_dir) - 1);
  if (!mkdtemp(inner_mount_dir)) {
    log_error("creating inner directory failed: %m");
    return -1;
  }

  log_debug("pivot root preparation complete");

  log_debug("pivot root...");
  if (pivot_root(mount_dir, inner_mount_dir)) {
    log_error("pivot root failed: %m");
    return -1;
  }
  log_debug("pivot root complete");

  log_debug("unmounting old root...");
  char *old_root_dir = basename(inner_mount_dir);
  char old_root[sizeof(inner_mount_dir) + 1] = {"/"};
  strlcpy(&old_root[1], old_root_dir, sizeof(old_root));

  log_debug("changing directory to /...");
  if (chdir("/")) {
    log_error("chdir failed: %m");
    return -1;
  }

  log_debug("unmounting...");
  if (umount2(old_root, MNT_DETACH)) {
    log_error("umount failed: %m");
    return -1;
  }

  log_debug("removing temporary directories...");
  if (rmdir(old_root)) {
    log_error("rmdir failed: %m");
    return -1;
  }

  log_debug("mounts setup complete");
  return 0;
}
