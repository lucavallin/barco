#include "sec.h"
#include "log.h"
#include <asm-generic/ioctls.h>
#include <linux/capability.h>
#include <linux/prctl.h>
#include <linux/sched.h>
#include <seccomp.h>
#include <stdio.h>
#include <sys/capability.h>
#include <sys/prctl.h>
#include <sys/stat.h>

// Capabilities are used to finely define the privileges of a process.
// The process' own inheritable set and bounding set of capabilities
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
//
// Notice: in some edge cases, some capabilities might not be respected because
// they are not namespaced (e.g. when writing to parts of procfs)
int sec_set_caps(void) {
  log_debug("setting capabilities...");
  int drop_caps[] = {
      CAP_AUDIT_CONTROL,   CAP_AUDIT_READ,   CAP_AUDIT_WRITE, CAP_BLOCK_SUSPEND,
      CAP_DAC_READ_SEARCH, CAP_FSETID,       CAP_IPC_LOCK,    CAP_MAC_ADMIN,
      CAP_MAC_OVERRIDE,    CAP_MKNOD,        CAP_SETFCAP,     CAP_SYSLOG,
      CAP_SYS_ADMIN,       CAP_SYS_BOOT,     CAP_SYS_MODULE,  CAP_SYS_NICE,
      CAP_SYS_RAWIO,       CAP_SYS_RESOURCE, CAP_SYS_TIME,    CAP_WAKE_ALARM};

  int num_caps = sizeof(drop_caps) / sizeof(*drop_caps);
  log_debug("dropping bounding capabilities...");
  for (int i = 0; i < num_caps; i++) {
    if (prctl(PR_CAPBSET_DROP, drop_caps[i], 0, 0, 0)) {
      log_error("failed to prctl cap %d: %m", drop_caps[i]);
      return 1;
    }
  }

  log_debug("dropping inheritable capabilities...");
  cap_t caps = NULL;
  if (!(caps = cap_get_proc()) ||
      cap_set_flag(caps, CAP_INHERITABLE, num_caps, drop_caps, CAP_CLEAR) ||
      cap_set_proc(caps)) {
    log_error("failed to run cap functions: %m");
    if (caps) {
      log_debug("freeing caps...");
      cap_free(caps);
    }

    return 1;
  }

  log_debug("freeing caps...");
  cap_free(caps);
  log_debug("capabilities set");

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
//
// New Linux syscalls are added to the kernel over time, so this list
// should be updated periodically.
int sec_set_seccomp(void) {
  scmp_filter_ctx ctx = NULL;

  log_debug("setting syscalls...");
  if (!(ctx = seccomp_init(SCMP_ACT_ALLOW)) ||
      // Calls that allow creating new setuid / setgid executables.
      // The contained process could created a setuid binary that can be used
      // by an user to get root in absence of user namespaces.
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(chmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID)) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(chmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID)) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(fchmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID)) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(fchmod), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID)) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(fchmodat), 1,
                       SCMP_A2(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID)) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(fchmodat), 1,
                       SCMP_A2(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID)) ||

      // Calls that allow contained processes to start new user namespaces
      // and possibly allow processes to gain new capabilities.
      seccomp_rule_add(
          ctx, SEC_SCMP_FAIL, SCMP_SYS(unshare), 1,
          SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER)) ||
      seccomp_rule_add(
          ctx, SEC_SCMP_FAIL, SCMP_SYS(clone), 1,
          SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER)) ||

      // Allows contained processes to write to the controlling terminal
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(ioctl), 1,
                       SCMP_A1(SCMP_CMP_MASKED_EQ, TIOCSTI, TIOCSTI)) ||

      // The kernel keyring system is not namespaced
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(keyctl), 0) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(add_key), 0) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(request_key), 0) ||

      // Before Linux 4.8, ptrace breaks seccomp
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(ptrace), 0) ||

      // Calls that let processes assign NUMA nodes. These could be used to deny
      // service to other NUMA-aware application on the host.
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(mbind), 0) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(migrate_pages), 0) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(move_pages), 0) ||
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(set_mempolicy), 0) ||

      // Alows userspace to handle page faults It can be used to pause execution
      // in the kernel by triggering page faults in system calls, a mechanism
      // often used in kernel exploits.
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(userfaultfd), 0) ||

      // This call could leak a lot of information on the host.
      // It can theoretically be used to discover kernel addresses and
      // uninitialized memory.
      seccomp_rule_add(ctx, SEC_SCMP_FAIL, SCMP_SYS(perf_event_open), 0) ||

      // Prevents setuid and setcap'd binaries from being executed
      // with additional privileges. This has some security benefits, but due to
      // weird side-effects, the ping command will not work in a process for
      // an unprivileged user.
      seccomp_attr_set(ctx, SCMP_FLTATR_CTL_NNP, 0) || seccomp_load(ctx)) {

    log_error("failed to set syscalls: %m");

    // Apply restrictions to the process and release the context.
    log_debug("releasing seccomp context...");
    if (ctx) {
      seccomp_release(ctx);
    }

    return 1;
  }

  log_debug("releasing seccomp context...");
  seccomp_release(ctx);
  log_debug("syscalls set");

  return 0;
}
