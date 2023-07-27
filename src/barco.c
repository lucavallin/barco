#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <linux/capability.h>
#include <linux/limits.h>
#include <pwd.h>
#include <sched.h>
#include <seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/capability.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

struct child_config {
  int argc;
  uid_t uid;
  int fd;
  char *hostname;
  char **argv;
  char *mount_dir;
};

#define STRTOL_BASE 10

int main(int argc, char **argv) {
  struct child_config config = {0};
  int err = 0;
  int option = 0;
  int sockets[2] = {0};
  int last_optind = 0;
  char *endptr;
  long int result = 0;

  while ((option = getopt(argc, argv, "c:m:u:"))) {
    switch (option) {
    case 'c':
      config.argc = argc - last_optind - 1;
      config.argv = &argv[argc - config.argc];
      goto finish_options;

    case 'm':
      config.mount_dir = optarg;
      break;

    case 'u':
      result = strtol(optarg, &endptr, STRTOL_BASE);

      // Check for errors in conversion
      if (*endptr != '\0' || endptr == optarg) {
        fprintf(stderr, "malformed uid: %s\n", optarg);
        goto usage;
      } else {
        config.uid = (int)result;
      }
      break;

    default:
      goto usage;
    }

    last_optind = optind;
  }

finish_options:
  if (!config.argc || !config.mount_dir) {
    goto usage;
  }

  // check-linux-version

  // hostname

  // namespaces

  goto exit;

usage:
  fprintf(stderr, "Usage: %s -u -1 -m . -c /bin/sh ~\n", argv[0]);
  goto error;

error:
  err = 1;

exit:
  if (sockets[0]) {
    close(sockets[0]);
  }

  if (sockets[1]) {
    close(sockets[1]);
  }

  return err;
}
