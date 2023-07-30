#include "../include/namespace.h"
#include "../lib/log.c/src/log.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// The container needs to send some messages to the parent, so we create a
// socket pair and give the container access to one.
int namespace_socket_pair_init(int sockets[]) {
  if (socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, sockets)) {
    log_error("socketpair failed: %m");
    return NAMESPACE_ERR_SOCKETPAIR;
  }
  if (fcntl(sockets[0], F_SETFD, FD_CLOEXEC)) {
    log_error("fcntl failed: %m");
    return NAMESPACE_ERR_FCNTL;
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
  if (!(*stack = malloc(NAMESPACE_STACK_SIZE))) {
    log_error("malloc failed");
    return NAMESPACE_ERR_STACK_MALLOC;
  }

  return 0;
}

int namespace_handle_container_uid_map(pid_t container_pid, int fd) {
  int uid_map = 0;
  int has_userns = -1;

  if (read(fd, &has_userns, sizeof(has_userns)) != sizeof(has_userns)) {
    log_error("could not read from container");
    return -1;
  }

  if (has_userns) {
    char path[PATH_MAX] = {0};
    for (char **file = (char *[]){"uid_map", "gid_map", 0}; *file; file++) {
      if (snprintf(path, sizeof(path), "/proc/%d/%s", container_pid, *file) >
          (int)sizeof(path)) {
        log_error("snprintf might be too big");
        return -1;
      }

      log_debug("writing %s...", path);
      if ((uid_map = open(path, O_WRONLY)) == -1) {
        log_error("open failed: %m");
        return -1;
      }
      if (dprintf(uid_map, "0 %d %d\n", NAMESPACE_USERNS_OFFSET,
                  NAMESPACE_USERNS_COUNT) == -1) {
        log_error("dprintf failed: %m");
        close(uid_map);
        return -1;
      }
      close(uid_map);
    }
  }

  if (write(fd, &(int){0}, sizeof(int)) != sizeof(int)) {
    log_error("could not write: %m");
    return -1;
  }

  return 0;
}
