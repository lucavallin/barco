#include "../include/cgroups.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct cgrp_control {
  char control[CGROUPS_CONTROL_FIELD_SIZE]; // Adjust the size based on your
                                            // needs
  struct cgrp_setting {
    char
        name[CGROUPS_CONTROL_FIELD_SIZE]; // Adjust the size based on your needs
    char value[CGROUPS_CONTROL_FIELD_SIZE]; // Adjust the size based on your
                                            // needs
  } **settings;
};

struct cgrp_setting add_to_tasks = {.name = "tasks", .value = "0"};

struct cgrp_control *cgrps[] = {
    &(struct cgrp_control){
        .control = "memory",
        .settings =
            (struct cgrp_setting *[]){
                &(struct cgrp_setting){.name = "memory.max",
                                       .value = CGROUP_MEMORY_MAX},
                &(struct cgrp_setting){.name = "memory.oom.max",
                                       .value = CGROUP_MEMORY_OOM_MAX},
                &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "cpu",
        .settings = (struct cgrp_setting *[]){&(struct cgrp_setting){
                                                  .name = "cpu.weight",
                                                  .value = CGROUP_CPU_WEIGHT},
                                              &add_to_tasks, NULL}},
    &(struct cgrp_control){
        .control = "pids",
        .settings = (struct cgrp_setting *[]){&(struct cgrp_setting){
                                                  .name = "pids.max",
                                                  .value = CGROUP_PIDS_MAX},
                                              &add_to_tasks, NULL}},
    NULL};

int cgroups_init(char *hostname) {
  // Create the cgroup mount point
  if (mount("cgroup2", "/sys/fs/cgroup", "cgroup2", 0, "")) {
    perror("mount cgroup2");
    return -1;
  }

  // Set resource limits using cgroupsv2 interface
  FILE *f;
  char path[PATH_MAX]; // Adjust the size based on your needs

  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    snprintf(path, sizeof(path), "/sys/fs/cgroup/%s/%s", (*cgrp)->control,
             hostname);

    if (mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR) && errno != EEXIST) {
      perror("mkdir");
      return -1;
    }

    for (struct cgrp_setting **setting = (*cgrp)->settings; *setting;
         setting++) {
      snprintf(path, sizeof(path), "/sys/fs/cgroup/%s/%s/%s", (*cgrp)->control,
               hostname, (*setting)->name);

      f = fopen(path, "w");
      if (!f) {
        perror("fopen");
        return -1;
      }
      if (fprintf(f, "%s", (*setting)->value) < 0) {
        perror("fprintf");
        fclose(f);
        return -1;
      }
      fclose(f);
    }
  }

  // The hard limit on the number of file descriptors is lowered.
  if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){
                                   .rlim_max = CGROUPS_FD_COUNT,
                                   .rlim_cur = CGROUPS_FD_COUNT,
                               })) {
    perror("setrlimit");
    return 1;
  }

  return 0;
}

int cgroups_free(char *hostname) {
  // Clean up the cgroup for the container
  FILE *f;
  char path[PATH_MAX]; // Adjust the size based on your needs

  for (struct cgrp_control **cgrp = cgrps; *cgrp; cgrp++) {
    snprintf(path, sizeof(path), "/sys/fs/cgroup/%s/%s", (*cgrp)->control,
             hostname);

    // Move the container process back into the root task (pid 1)
    f = fopen(path, "w");
    if (!f) {
      perror("fopen");
      return -1;
    }
    if (fprintf(f, "0") < 0) {
      perror("fprintf");
      fclose(f);
      return -1;
    }
    fclose(f);

    // Remove the process tree from the cgroup
    if (rmdir(path)) {
      perror("rmdir");
      return -1;
    }
  }

  // Unmount the cgroup hierarchy
  if (umount("/sys/fs/cgroup")) {
    perror("umount");
    return -1;
  }

  return 0;
}
