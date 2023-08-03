#include "cgroups.h"
#include "log.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
struct cgrp_control {
  char control[CGROUPS_CONTROL_FIELD_SIZE];
  struct cgrp_setting {
    char name[CGROUPS_CONTROL_FIELD_SIZE];
    char value[CGROUPS_CONTROL_FIELD_SIZE];
  } **settings;
};

struct cgrp_setting add_to_tasks = {.name = "cgroup.procs", .value = "0"};

// Cgroups v2 settings:
// - memory.max: 1GB (process memory limit)
// - cpu.weight: 256 (a quarter of the CPU time)
// - pids.max: 64 (max number of processes)
struct cgrp_control *cgrps[] = {
    &(struct cgrp_control){
        .control = "memory",
        .settings = (struct cgrp_setting *[]){&(struct cgrp_setting){
                                                  .name = "memory.max",
                                                  .value = "1000000000"},
                                              &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "cpu",
        .settings =
            (struct cgrp_setting *[]){
                &(struct cgrp_setting){.name = "cpu.weight", .value = "256"},
                &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "pids",
        .settings =
            (struct cgrp_setting *[]){
                &(struct cgrp_setting){.name = "pids.max", .value = "64"},
                &add_to_tasks, NULL}},
    NULL};

// cgroups settings are written to the cgroups v2 filesystem as follows:
// - create a directory for the cgroups
// - write the settings to the cgroups files (each setting is a file)
// - a pid can be added to cgroup.procs to add the process to the cgroups
int cgroups_init(char *hostname) {
  log_debug("setting cgroups...");

  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    char dir[PATH_MAX] = {0};
    log_debug("setting %s...", (*cgrp)->control);
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s/%s", (*cgrp)->control,
                 hostname) == -1) {
      log_error("failed to setup path: %m");
      return -1;
    }

    log_debug("creating %s...", dir);
    if (mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR)) {
      log_error("failed to mkdir %s: %m", dir);
      return -1;
    }

    log_debug("writing settings...");
    for (struct cgrp_setting **setting = (*cgrp)->settings; *setting;
         setting++) {
      char path[PATH_MAX] = {0};
      int fd = 0;

      log_debug("setting %s...", (*setting)->name);
      if (snprintf(path, sizeof(path), "%s/%s", dir, (*setting)->name) == -1) {
        return -1;
      }

      log_debug("opening %s...", path);
      if ((fd = open(path, O_WRONLY)) == -1) {
        log_error("failed to open %s: %m", path);
        return -1;
      }

      log_debug("writing %s to setting", (*setting)->value);
      if (write(fd, (*setting)->value, strlen((*setting)->value)) == -1) {
        log_error("failed to write %s: %m", path);
        close(fd);
        return -1;
      }
      close(fd);
    }
  }

  log_debug("cgroups set");
  return 0;
}

// Clean up the cgroups for the process: the container process is moved back
// into the root task. When the process exits, its process tree is removed and
// the task is empty. Afterwards, rmdir can safely be run.
// There are Linux-native ways to do this automatically, but it would require
// changing system-wide settings.
int cgroups_free(char *hostname) {
  log_debug("freeing cgroups...");
  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    char dir[PATH_MAX] = {0};
    char task[PATH_MAX] = {0};
    int task_fd = 0;

    log_debug("freeing %s...", (*cgrp)->control);
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s/%s", (*cgrp)->control,
                 hostname) == -1 ||
        snprintf(task, sizeof(task), "/sys/fs/cgroup/%s/cgroup.procs",
                 (*cgrp)->control) == -1) {
      log_error("failed to setup paths: %m");
      return -1;
    }

    log_debug("opening %s...", task);
    if ((task_fd = open(task, O_WRONLY)) == -1) {
      log_error("failed to open %s: %m", task);
      return -1;
    }

    log_debug("writing to %s...", task);
    if (write(task_fd, "0", 2) == -1) {
      log_error("failed to write %s: %m", task);
      close(task_fd);
      return -1;
    }
    close(task_fd);

    log_debug("removing %s...", dir);
    if (rmdir(dir)) {
      log_error("failed to rmdir %s: %m", dir);
      return -1;
    }
  }

  log_debug("cgroups released");
  return 0;
}
