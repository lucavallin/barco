#include "../include/cgroups.h"
#include "../lib/log/src/log.h"
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

struct cgrp_setting add_to_tasks = {.name = "tasks", .value = "0"};

// Cgroups let us limit resources allocated to a process to prevent it from
// dying services to the rest of the system. The cgroups must be created before
// the container enters a cgroups namespace. The following settings are applied:
// - memory.limit_in_bytes: 1GB (container memory limit)
// - memory.kmem.limit_in_bytes: 1GB (kernel memory limit)
// - cpu.shares: 256 (a quarter of the CPU time)
// - pids.max: 64 (max number of processes)
// - blkio.weight: 50 (I/O priority, lower than the rest of the system and
// prioritized accordingly)
struct cgrp_control *cgrps[] = {
    &(struct cgrp_control){
        .control = "memory",
        .settings =
            (struct cgrp_setting *[]){
                &(struct cgrp_setting){.name = "memory.limit_in_bytes",
                                       .value = CGROUPS_MEMORY},
                &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "cpu",
        .settings = (struct cgrp_setting *[]){&(struct cgrp_setting){
                                                  .name = "cpu.shares",
                                                  .value = CGROUPS_SHARES},
                                              &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "pids",
        .settings = (struct cgrp_setting *[]){
            &(struct cgrp_setting){.name = "pids.max", .value = CGROUPS_PIDS},
            &add_to_tasks, NULL}}};

// cgroups settings are written to the cgroups v1 filesystem as follows:
// - create a directory for the cgroups
// - write the settings to the cgroups files (each setting is a file)
// - a pid can be added to tasks to add the process tree to the cgroups (pid 0
// means the writing process)
int cgroups_init(char *hostname) {
  log_debug("setting cgroups...");

  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    char dir[PATH_MAX] = {0};

    log_debug("%s...", (*cgrp)->control);
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroups/%s/%s", (*cgrp)->control,
                 hostname) == -1) {
      return -1;
    }

    if (mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR)) {
      log_error("mkdir %s failed: %m\n", dir);
      return -1;
    }

    for (struct cgrp_setting **setting = (*cgrp)->settings; *setting;
         setting++) {
      char path[PATH_MAX] = {0};
      int fdr = 0;
      if (snprintf(path, sizeof(path), "%s/%s", dir, (*setting)->name) == -1) {
        log_error("snprintf failed: %m");
        return -1;
      }

      fdr = open(path, O_WRONLY);
      if (fdr == -1) {
        log_error("opening %s failed: %m\n", path);
        return -1;
      }
      if (write(fdr, (*setting)->value, strlen((*setting)->value)) == -1) {
        log_error("writing to %s failed: %m\n", path);
        close(fdr);
        return -1;
      }

      close(fdr);
    }
  }
  log_debug("done");

  return 0;
}

// Clean up the cgroups for the container: the container process is moved back
// into the root task. When the container exits, its process tree is removed and
// the task is empty. Afterwards, rmdir can safely be run.
int cgroups_free(char *hostname) {
  log_debug("cleaning cgroups...");
  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    char dir[PATH_MAX] = {0};
    char task[PATH_MAX] = {0};
    int task_fd = 0;

    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroups/%s/%s", (*cgrp)->control,
                 hostname) == -1 ||
        snprintf(task, sizeof(task), "/sys/fs/cgroups/%s/tasks",
                 (*cgrp)->control) == -1) {
      log_error("snprintf failed: %m");
      return -1;
    }

    task_fd = open(task, O_WRONLY);
    if (task_fd == -1) {
      log_error("opening %s failed: %m", task);
      return -1;
    }
    if (write(task_fd, "0", 2) == -1) {
      log_error("writing to %s failed: %m", task);
      close(task_fd);
      return -1;
    }

    close(task_fd);

    if (rmdir(dir)) {
      log_error("rmdir %s failed: %m", dir);
      return -1;
    }
  }
  log_debug("done");
  return 0;
}
