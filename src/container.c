// Required for clone()
#define _GNU_SOURCE
#include "../include/container.h"
#include "../lib/log.c/src/log.h"
#include <errno.h>
#include <grp.h>
#include <sched.h>
#include <seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/capability.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

// Listens for the container to request setting uid and gid mappings.
// If successful, isetgroups, setresgid, and setresuid are called.
// setgroups and setresgid are necessary because of two separate group
// mechanisms on Linux. The function assumes that every uid has a corresponding
// gid, which is often the case.
int container_set_userns(container_config *config) {
  int has_userns = !unshare(CLONE_NEWUSER);
  int result = 0;

  fprintf(stderr, "=> trying a user namespace...");
  if (write(config->fd, &has_userns, sizeof(has_userns)) !=
      sizeof(has_userns)) {
    fprintf(stderr, "couldn't write: %m\n");
    return -1;
  }

  if (read(config->fd, &result, sizeof(result)) != sizeof(result)) {
    fprintf(stderr, "couldn't read: %m\n");
    return -1;
  }
  if (result) {
    return -1;
  }

  if (has_userns) {
    fprintf(stderr, "done.\n");
  } else {
    fprintf(stderr, "unsupported? continuing.\n");
  }

  fprintf(stderr, "=> switching to uid %d / gid %d...", config->uid,
          config->uid);

  if (setgroups(1, &(gid_t){config->uid}) ||
      setresgid(config->uid, config->uid, config->uid) ||
      setresuid(config->uid, config->uid, config->uid)) {
    fprintf(stderr, "%m\n");
    return -1;
  }

  fprintf(stderr, "done.\n");

  return 0;
}

// Capabilities are used to define the privileges of a process.
// In some edge cases, capabilities might not be respected.
// The container's own inheritable set and bounding set of capabilities
// should be dropped before setting them to the desired values.
//
// The following capabilities are dropped:
// CAP_AUDIT_CONTROL, _READ, and _WRITE: allow access to the kernel audit system
// CAP_BLOCK_SUSPEND: allow preventing system suspend
// CAP_DAC_READ_SEARCH: allow bypassing file/dir permission checks
// CAP_FSETID: allow setting arbitrary process IDs
// CAP_IPC_LOCK: allow locking memory
// CAP_MAC_ADMIN and _OVERRIDE: allow MAC configuration or state changes
// CAP_MKNOD: allow creating special files using mknod()
// CAP_SETFCAP: allow setting arbitrary capabilities
// CAP_SYSLOG: allow reading and writing kernel logs
// CAP_SYS_ADMIN: allow configuring the system
// CAP_SYS_BOOT: allow rebooting the system
// CAP_SYS_MODULE: allow loading and unloading kernel modules
// CAP_SYS_NICE: allow raising process priority
// CAP_SYS_RAWIO: allow raw I/O operations
// CAP_SYS_RESOURCE: allow configuring resource limits
// CAP_SYS_TIME: allow setting system time
// CAP_WAKE_ALARM: allow waking the system from suspend
//
// The following capabilities are retained:
// CAP_DAC_OVERRIDE: allow bypassing file/dir permission checks inside the
// mount namespace
// CAP_FOWNER, CAP_LEASE, and CAP_LINUX_IMMUTABLE: allow file/dir actions inside
// the mount namespace
// CAP_SYS_PACCT: allow configuring process accounting
// CAP_IPC_OWNER: allow IPC object configuration inside namespace
// CAP_NET_ADMIN: allow configuring network interfaces
// CAP_NET_BIND_SERVICE: allow binding to ports below 1024
// CAP_NET_RAW: allow raw socket operations
// CAP_SYS_PTRACE: allow ptrace(), but not across pid namespaces
// CAP_KILL: allow sending signals to processes, but not across pid namespaces
// CAP_SETUID and CAPSETGID: allow setting arbitrary uids/gids
// CAP_SETPCAP: allow setting arbitrary, already-assigned capabilities
// CAP_SYS_CHROOT: allow chroot() (has risks)
// CAP_SYS_TTYCONFIG: allow configuring TTY devices (has risks)
int container_set_capabilities() {
  fprintf(stderr, "=> dropping capabilities...");
  int drop_caps[] = {
      CAP_AUDIT_CONTROL,   CAP_AUDIT_READ,   CAP_AUDIT_WRITE, CAP_BLOCK_SUSPEND,
      CAP_DAC_READ_SEARCH, CAP_FSETID,       CAP_IPC_LOCK,    CAP_MAC_ADMIN,
      CAP_MAC_OVERRIDE,    CAP_MKNOD,        CAP_SETFCAP,     CAP_SYSLOG,
      CAP_SYS_ADMIN,       CAP_SYS_BOOT,     CAP_SYS_MODULE,  CAP_SYS_NICE,
      CAP_SYS_RAWIO,       CAP_SYS_RESOURCE, CAP_SYS_TIME,    CAP_WAKE_ALARM};

  size_t num_caps = sizeof(drop_caps) / sizeof(*drop_caps);
  fprintf(stderr, "bounding...");
  for (size_t i = 0; i < num_caps; i++) {
    if (prctl(PR_CAPBSET_DROP, drop_caps[i], 0, 0, 0)) {
      fprintf(stderr, "prctl failed: %m\n");
      return 1;
    }
  }

  fprintf(stderr, "inheritable...");
  cap_t caps = NULL;
  if (!(caps = cap_get_proc()) ||
      cap_set_flag(caps, CAP_INHERITABLE, num_caps, drop_caps, CAP_CLEAR) ||
      cap_set_proc(caps)) {

    fprintf(stderr, "failed: %m\n");
    if (caps) {
      cap_free(caps);
    }

    return 1;
  }

  cap_free(caps);
  fprintf(stderr, "done.\n");

  return 0;
}

// pivot_root is a system call to swap the mount at / with another.
// glibc does not provide a wrapper for it.
long pivot_root(const char *new_root, const char *put_old) {
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
int container_set_mounts(container_config *config) {
  // MS_PRIVATE makes the bind mount invisible outside of the namespace
  fprintf(stderr, "=> remounting everything with MS_PRIVATE...");
  if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL)) {
    fprintf(stderr, "failed! %m\n");
    return -1;
  }
  fprintf(stderr, "remounted.\n");

  fprintf(stderr, "=> making a temp directory and a bind mount there...");
  char mount_dir[] = "/tmp/tmp.XXXXXX";
  if (!mkdtemp(mount_dir)) {
    fprintf(stderr, "failed making a directory!\n");
    return -1;
  }

  if (mount(config->mount_dir, mount_dir, NULL, MS_BIND | MS_PRIVATE, NULL)) {
    fprintf(stderr, "bind mount failed: path=%s\n", config->mount_dir);
    return -1;
  }

  char inner_mount_dir[] = "/tmp/tmp.XXXXXX/oldroot.XXXXXX";
  memcpy(inner_mount_dir, mount_dir, sizeof(mount_dir) - 1);
  if (!mkdtemp(inner_mount_dir)) {
    fprintf(stderr, "failed making the inner directory!\n");
    return -1;
  }
  fprintf(stderr, "done.\n");

  fprintf(stderr, "=> pivoting root...");
  if (pivot_root(mount_dir, inner_mount_dir)) {
    fprintf(stderr, "failed!\n");
    return -1;
  }
  fprintf(stderr, "done.\n");

  char *old_root_dir = basename(inner_mount_dir);
  char old_root[sizeof(inner_mount_dir) + 1] = {"/"};
  strcpy(&old_root[1], old_root_dir);

  fprintf(stderr, "=> unmounting %s...", old_root);
  if (chdir("/")) {
    fprintf(stderr, "chdir failed! %m\n");
    return -1;
  }
  if (umount2(old_root, MNT_DETACH)) {
    fprintf(stderr, "umount failed! %m\n");
    return -1;
  }
  if (rmdir(old_root)) {
    fprintf(stderr, "rmdir failed! %m\n");
    return -1;
  }
  fprintf(stderr, "done.\n");
  return 0;
}

// Blocks sensitive system calls based on Docker's default seccomp profile
// and other obsolete or dangerous system calls.
// The syscalls disallowed by the default Docker policy but permitted by this
// code are:
// - get_mempolicy, getpagesize, pciconfig_iobase, ustat, sysfs: These
// reveal information abour memory layout, PCI devices, filesystem
// - uselib: allows loading a shared library in userspace
//
// Other syscalls are (either):
// - already prevented by the capabilities set
// - available only on particular architectures
// - newer versions or aliases of other syscalls
int container_set_syscalls() {
  scmp_filter_ctx ctx = NULL;
  fprintf(stderr, "=> filtering syscalls...");
  if (!(ctx = seccomp_init(SCMP_ACT_ALLOW)) ||
      // Calls that allow creating new setuid / setgid executables.
      // The contained process could created a setuid binary that can be used
      // by an user to get root in absence of user namespaces.
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(chmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID)) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(chmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID)) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(fchmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID)) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(fchmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID)) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(fchmodat), 1,
                       SCMP_A2(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID)) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(fchmodat), 1,
                       SCMP_A2(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID)) ||

      // Calls that allow contained processes to start new user namespaces
      // and possibly allow processes to gain new capabilities.
      seccomp_rule_add(
          ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(unshare), 1,
          SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER)) ||
      seccomp_rule_add(
          ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(clone), 1,
          SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER)) ||

      // Allows contained processes to write to the controlling terminal
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(ioctl), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, TIOCSTI, TIOCSTI)) ||

      // The kernel keyring system is not namespaced
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(keyctl), 0) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(add_key), 0) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(request_key), 0) ||

      // Before Linux 4.8, ptrace breaks seccomp
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(ptrace), 0) ||

      // Calls that let processes assign NUMA nodes. These could be used to deny
      // service to other NUMA-aware application on the host.
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(mbind), 0) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(migrate_pages), 0) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(move_pages), 0) ||
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(set_mempolicy), 0) ||

      // Alows userspace to handle page faults It can be used to pause execution
      // in the kernel by triggering page faults in system calls, a mechanism
      // often used in kernel exploits.
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(userfaultfd), 0) ||

      // This call could leak a lot of information on the host.
      // It can theoretically be used to discover kernel addresses and
      // uninitialized memory.
      seccomp_rule_add(ctx, CONTAINER_SCMP_FAIL, SCMP_SYS(perf_event_open),
                       0) ||

      // Prevents setuid and setcap'd binaries from being executed
      // with additional privileges. This has some security benefits, but due to
      // weird side-effects, the ping command will not work in a container for
      // an unprivileged user.
      seccomp_attr_set(ctx, SCMP_FLTATR_CTL_NNP, 0) || seccomp_load(ctx)) {

    // Apply restrictions to the process and release the context.
    if (ctx) {
      seccomp_release(ctx);
    }

    fprintf(stderr, "failed: %m\n");
    return 1;
  }

  seccomp_release(ctx);
  fprintf(stderr, "done.\n");

  return 0;
}

// The order of the operations is of important as, for example,
// mounts cannot be changed without specific capabilities,
// unshare cannot be called after syscalls are limited, etc...
int container_start(void *arg) {
  container_config *config = arg;
  if (sethostname(config->hostname, strlen(config->hostname)) ||
      container_set_mounts(config) || container_set_userns(config) ||
      container_set_capabilities() || container_set_syscalls()) {
    close(config->fd);
    return -1;
  }
  if (close(config->fd)) {
    fprintf(stderr, "close failed: %m\n");
    return -1;
  }
  if (execve(&config->cmd[0], &config->cmd, NULL)) {
    fprintf(stderr, "execve failed! %m.\n");
    return -1;
  }
  return 0;
}

// Creates container (process) with different properties than its parent
// e.g. mount to different dir, different hostname, etc...
// All these requirements are specified by the flags we pass to clone()
int container_init(container_config *config, char *stack) {
  // The flags namespace the mounts, pids, IPC data structures, network devices
  // and hostname.
  int flags = CLONE_NEWNS | CLONE_NEWCGROUP | CLONE_NEWPID | CLONE_NEWIPC |
              CLONE_NEWNET | CLONE_NEWUTS;

  // SIGCHLD lets us wait on the child process.
  int container_pid = clone(container_start, stack, flags | SIGCHLD, &config);

  // Close and zero the child's socket, to avoid leaving an open fd in
  // case of a failure. If we don't do this, the child or parent might hang.
  close(config->fd);
  config->fd = 0;

  return container_pid;
}

void container_stop(int container_pid) { kill(container_pid, SIGKILL); }

int container_destroy(int container_pid) {
  int child_status = 0;
  waitpid(container_pid, &child_status, 0);
  return WEXITSTATUS(child_status);
}

void container_hostname_generate(char *hostname) {
  int len = sizeof(hostname);
  char chars[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

  srand(time(0));
  for (int i = 0; i < len; i++) {
    int chars_len = sizeof(chars) - 1;
    int randomIndex =
        (int)rand() % chars_len; // NOLINT(cert-msc30-c, cert-msc50-cpp)
    hostname[i] = chars[randomIndex];
  }
  // null terminate the string
  hostname[len] = '\0';
}
