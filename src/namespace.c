#include "../include/namespace.h"
#include "../lib/log.c/src/log.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// The container needs to send some messages to the parent, so we create a
// socket pair and give the container access to one.
int namespace_socket_pair_init(int sockets[]) {
  if (socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, sockets)) {
    log_error("socketpair failed: %m");
    return -1;
  }
  if (fcntl(sockets[0], F_SETFD, FD_CLOEXEC)) {
    log_error("fcntl failed: %m");
    return -1;
  }

  return 0;
}

void namespace_socket_pair_close(int sockets[]) {
  if (sockets[0]) {
    close(sockets[0]);
  }

  if (sockets[1]) {
    close(sockets[1]);
  }
}

int namespace_stack_init(char **stack) {
  *stack = malloc(NAMESPACE_STACK_SIZE);
  if (!*stack) {
    log_error("malloc failed");
    return -1;
  }

  return 0;
}

int namespace_set_uid_map(pid_t container_pid, int fdr) {
  int uid_map = 0;
  char path[PATH_MAX] = {0};

  for (char **file = (char *[]){"uid_map", "gid_map", 0}; *file; file++) {
    if (snprintf(path, sizeof(path), "/proc/%d/%s", container_pid, *file) >
        (int)sizeof(path)) {
      log_error("snprintf might be too big");
      return -1;
    }

    log_debug("writing %s...", path);
    uid_map = open(path, O_WRONLY);
    if (uid_map == -1) {
      log_error("open failed: %m");
      return -1;
    }

    // if (dprintf(uid_map, "0 %d %d\n", NAMESPACE_USERNS_OFFSET,
    //             NAMESPACE_USERNS_COUNT) == -1) {
    //   log_error("dprintf failed: %m");
    //   close(uid_map);
    //   return -1;
    // }

    close(uid_map);
  }

  if (write(fdr, &(int){0}, sizeof(int)) != sizeof(int)) {
    log_error("could not write: %m");
    return -1;
  }

  return 0;
}
