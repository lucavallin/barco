#include "cgroups.h"
#include "log.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// This struct is used to store cgroups settings.
struct cgroups_setting {
  char name[CGROUPS_CONTROL_FIELD_SIZE];
  char value[CGROUPS_CONTROL_FIELD_SIZE];
};

// cgroups settings are written to the cgroups v2 filesystem as follows:
// - create a directory for the new cgroup
// - settings files are created automatically
// - write the settings to the corresponding files
int cgroups_init(char *hostname, pid_t pid) {
  char cgroup_dir[PATH_MAX] = {0};

  // The "cgroup.procs" setting is used to add a process to a cgroup.
  // It is prepared here with the pid of the calling process, so that it can be
  // added to the cgroup later.
  struct cgroups_setting *procs_setting =
      &(struct cgroups_setting){.name = CGROUPS_CGROUP_PROCS, .value = ""};
  ;
  if (snprintf(procs_setting->value, CGROUPS_CONTROL_FIELD_SIZE, "%d", pid) ==
      -1) {
    log_error("failed to setup cgroup.procs setting: %m");
    return -1;
  }
  // Cgroups let us limit resources allocated to a process to prevent it from
  // dying services to the rest of the system. The cgroups must be created
  // before the process enters a cgroups namespace. The following settings are
  // applied:
  // - memory.limit_in_bytes: 1GB (process memory limit)
  // - cpu.shares: 256 (a quarter of the CPU time)
  // - pids.max: 64 (max number of processes)
  // - cgroup.procs: 0 (the calling process is added to the cgroup)
  struct cgroups_setting *cgroups_setting_list[] = {
      &(struct cgroups_setting){.name = "memory.max",
                                .value = CGROUPS_MEMORY_MAX},
      &(struct cgroups_setting){.name = "cpu.weight",
                                .value = CGROUPS_CPU_WEIGHT},
      &(struct cgroups_setting){.name = "pids.max", .value = CGROUPS_PIDS_MAX},
      procs_setting, NULL};

  log_debug("setting cgroups...");

  // Create the cgroup directory.
  if (snprintf(cgroup_dir, sizeof(cgroup_dir), "/sys/fs/cgroup/%s", hostname) ==
      -1) {
    log_error("failed to setup path: %m");
    return -1;
  }

  log_debug("creating %s...", cgroup_dir);
  if (mkdir(cgroup_dir, S_IRUSR | S_IWUSR | S_IXUSR)) {
    log_error("failed to mkdir %s: %m", cgroup_dir);
    return -1;
  }

  // Loop through and write settings to the corresponding files in the cgroup
  // directory.
  for (struct cgroups_setting **setting = cgroups_setting_list; *setting;
       setting++) {
    char setting_dir[PATH_MAX] = {0};
    int fd = 0;

    log_info("setting %s to %s...", (*setting)->name, (*setting)->value);
    if (snprintf(setting_dir, sizeof(setting_dir), "%s/%s", cgroup_dir,
                 (*setting)->name) == -1) {
      log_error("failed to setup path: %m");
      return -1;
    }

    log_debug("opening %s...", setting_dir);
    if ((fd = open(setting_dir, O_WRONLY)) == -1) {
      log_error("failed to open %s: %m", setting_dir);
      return -1;
    }

    log_debug("writing %s to setting", (*setting)->value);
    if (write(fd, (*setting)->value, strlen((*setting)->value)) == -1) {
      log_error("failed to write %s: %m", setting_dir);
      close(fd);
      return -1;
    }

    log_debug("closing %s...", setting_dir);
    if (close(fd)) {
      log_error("failed to close %s: %m", setting_dir);
      return -1;
    }
  }

  log_debug("cgroups set");
  return 0;
}

// Clean up the cgroups for the process. Since barco write the PID of its child
// process to the cgroup.procs file, all that is needed is to remove the cgroups
// directory after the child process is exited.
int cgroups_free(char *hostname) {
  char dir[PATH_MAX] = {0};

  log_debug("freeing cgroups...");

  if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s", hostname) == -1) {
    log_error("failed to setup paths: %m");
    return -1;
  }

  log_debug("removing %s...", dir);
  if (rmdir(dir)) {
    log_error("failed to rmdir %s: %m", dir);
    return -1;
  }

  log_debug("cgroups released");
  return 0;
}
