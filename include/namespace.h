#pragma once
#include <sys/types.h>

// The stack size for the container
#define NAMESPACE_STACK_SIZE (1024 * 1024)

// Initializes a socket pair for communication
// with the container
int namespace_socket_pair_init(int sockets[]);

// Initializes a stack for the container
int namespace_stack_init(char **stack);

// Closes the socket pair
void namespace_socket_pair_close(int sockets[]);

// Configures the container's user namespace and
// pause until its process tree exits
int namespace_handle_container_uid_map(pid_t container_pid, int fd);
