#ifndef SEC_H
#define SEC_H

#include <errno.h>

// Used to represent the result of a seccomp rule.
#define SEC_SCMP_FAIL SCMP_ACT_ERRNO(EPERM)

// Setup capabilities for the calling process
int sec_set_caps(void);

// Setup seccomp for the calling process
int sec_set_seccomp(void);

#endif
