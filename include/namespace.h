#pragma once
#include <sys/types.h>

// The stack size for the container
#define NAMESPACE_STACK_SIZE (1024 * 1024)
// Error codes
#define NAMESPACE_ERR_SOCKETPAIR 1
#define NAMESPACE_ERR_FCNTL 2
#define NAMESPACE_ERR_STACK_MALLOC 3
#define NAMESPACE_USERNS_OFFSET 10000
#define NAMESPACE_USERNS_COUNT 2000

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
