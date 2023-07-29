#include "../include/cgroup.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>

#define CGROUP_MEMORY "1073741824" // 1GB
#define CGROUP_SHARES "256"
#define CGROUP_PIDS "64"
#define CGROUP_WEIGHT "10"
#define CGROUP_FD_COUNT 64
#define CGROUP_CONTROL_FIELD_SIZE 256

struct cgrp_control {
  char control[CGROUP_CONTROL_FIELD_SIZE];
  struct cgrp_setting {
    char name[CGROUP_CONTROL_FIELD_SIZE];
    char value[CGROUP_CONTROL_FIELD_SIZE];
  } * *settings;
};

struct cgrp_setting add_to_tasks = {.name = "tasks", .value = "0"};

// Cgroups let us limit resources allocated to a process to prevent it from
// dying services to the rest of the system. The cgroup must be created before
// the container enters a cgroup namespace. The following settings are applied:
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
                                       .value = CGROUP_MEMORY},
                &(struct cgrp_setting){.name = "memory.kmem.limit_in_bytes",
                                       .value = CGROUP_MEMORY},
                &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "cpu",
        .settings = (struct cgrp_setting *[]){&(struct cgrp_setting){
                                                  .name = "cpu.shares",
                                                  .value = CGROUP_SHARES},
                                              &add_to_tasks, NULL}},
    &(struct cgrp_control){.control = "pids",
                           .settings =
                               (struct cgrp_setting *[]){
                                   &(struct cgrp_setting){.name = "pids.max",
                                                          .value = CGROUP_PIDS},
                                   &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "blkio",
        .settings = (struct cgrp_setting *[]){&(struct cgrp_setting){
                                                  .name = "blkio.weight",
                                                  .value = CGROUP_PIDS},
                                              &add_to_tasks, NULL}},
    NULL};

// cgroup settings are written to the cgroup v1 filesystem as follows:
// - create a directory for the cgroup
// - write the settings to the cgroup files (each setting is a file)
// - a pid can be added to tasks to add the process tree to the cgroup (pid 0
// means the writing process)
int cgroup_init(container_config *config) {
  fprintf(stderr, "=> setting cgroup...");
  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    char dir[PATH_MAX] = {0};
    fprintf(stderr, "%s...", (*cgrp)->control);
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s/%s", (*cgrp)->control,
                 config->hostname) == -1) {
      return -1;
    }
    if (mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR)) {
      fprintf(stderr, "mkdir %s failed: %m\n", dir);
      return -1;
    }
    for (struct cgrp_setting **setting = (*cgrp)->settings; *setting;
         setting++) {
      char path[PATH_MAX] = {0};
      int fd = 0;
      if (snprintf(path, sizeof(path), "%s/%s", dir, (*setting)->name) == -1) {
        fprintf(stderr, "snprintf failed: %m\n");
        return -1;
      }
      if ((fd = open(path, O_WRONLY)) == -1) {
        fprintf(stderr, "opening %s failed: %m\n", path);
        return -1;
      }
      if (write(fd, (*setting)->value, strlen((*setting)->value)) == -1) {
        fprintf(stderr, "writing to %s failed: %m\n", path);
        close(fd);
        return -1;
      }
      close(fd);
    }
  }
  fprintf(stderr, "done.\n");

  // The hard limit on the number of file descriptors is lowered. If the
  // capability CAP_SYS_RESOURCE is dropped, the limit is permanent for
  // this process tree. The number of file descriptors is on a per-user basis.
  // This limit prevents in-container process from occupying all of them.
  fprintf(stderr, "=> setting rlimit...");
  if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){
                                   .rlim_max = CGROUP_FD_COUNT,
                                   .rlim_cur = CGROUP_FD_COUNT,
                               })) {
    fprintf(stderr, "failed: %m\n");
    return 1;
  }
  fprintf(stderr, "done.\n");
  return 0;
}

// Clean up the cgroup for the container: the container process is moved back
// into the root task. When the container exits, its process tree is removed and
// the task is empty. Afterwards, rmdir can safely be run.
int cgroup_free(container_config *config) {
  fprintf(stderr, "=> cleaning cgroup...");
  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    char dir[PATH_MAX] = {0};
    char task[PATH_MAX] = {0};
    int task_fd = 0;
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s/%s", (*cgrp)->control,
                 config->hostname) == -1 ||
        snprintf(task, sizeof(task), "/sys/fs/cgroup/%s/tasks",
                 (*cgrp)->control) == -1) {
      fprintf(stderr, "snprintf failed: %m\n");
      return -1;
    }
    if ((task_fd = open(task, O_WRONLY)) == -1) {
      fprintf(stderr, "opening %s failed: %m\n", task);
      return -1;
    }
    if (write(task_fd, "0", 2) == -1) {
      fprintf(stderr, "writing to %s failed: %m\n", task);
      close(task_fd);
      return -1;
    }
    close(task_fd);
    if (rmdir(dir)) {
      fprintf(stderr, "rmdir %s failed: %m", dir);
      return -1;
    }
  }
  fprintf(stderr, "done.\n");
  return 0;
}
