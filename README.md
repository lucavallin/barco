# <img src="docs/barco.png" style="width:40px;padding-right:10px;margin-bottom:-8px;"> barco

`barco` is a project I worked on to learn more about Linux containers and the Linux kernel, based on other guides on the internet.
Linux containers are made up by a set of Linux kernel features:

- `namespaces`: are used to group kernel objects into different sets that can be accessed by specific process trees. There are different types of `namespaces`, for example,the `PID` namespace is used to isolate the process tree, while the `network` namespace is used to isolate the network stack.
- `seccomp`: is used to limit the system calls that a process can make (handled via syscalls)
- `capabilities`: are used to set limits on what uid 0 (root) can do (handled via syscalls)
- `cgroups`: are used to limit the resources (e.g. memory, disk I/O, CPU-tme) that a process can use (handled via cgroupfs)

## Usage

`barco` can be used to run `bin/sh . ` from the `/` directory as `root` (-u 0) with the following command (optional `-v` for verbose output):

```bash
$ sudo ./bin/barco -u 0 -m / -c /bin/sh -a . [-v]

22:08:41 INFO  ./src/barco.c:96: initializing socket pair...
22:08:41 INFO  ./src/barco.c:103: setting socket flags...
22:08:41 INFO  ./src/barco.c:112: initializing container stack...
22:08:41 INFO  ./src/barco.c:120: initializing container...
22:08:41 INFO  ./src/barco.c:131: initializing cgroups...
22:08:41 INFO  ./src/cgroups.c:73: setting memory.max to 1G...
22:08:41 INFO  ./src/cgroups.c:73: setting cpu.weight to 256...
22:08:41 INFO  ./src/cgroups.c:73: setting pids.max to 64...
22:08:41 INFO  ./src/cgroups.c:73: setting cgroup.procs to 1458...
22:08:41 INFO  ./src/barco.c:139: configuring user namespace...
22:08:41 INFO  ./src/barco.c:147: waiting for container to exit...
22:08:41 INFO  ./src/container.c:43: ### BARCONTAINER STARTING - type 'exit' to quit ###

# ls
bin         home                lib32       media       root        sys         vmlinuz
boot        initrd.img          lib64       mnt         run         tmp         vmlinuz.old
dev         initrd.img.old      libx32      opt         sbin        usr
etc         lib                 lost+found  proc        srv         var
# echo "i am a container"
i am a container
# exit

22:08:55 INFO  ./src/barco.c:153: freeing resources...
22:08:55 INFO  ./src/barco.c:168: so long and thanks for all the fish
```

## Setup

`barco` requires a number of tools and libraries to be installed to build the project and for development.

```bash
# Install all required tooling and dependencies
$ sudo apt install -y make
$ make setup
```

### Dependencies

`barco` depends on the following "non-standard" libraries:

- `libseccomp`: used to set up seccomp filters
- `libcap`: used to set container capabilities
- `libbsd`: used for `strlcpy`
- `libcuni1`: used for testing with CUnit
- [argtable](http://argtable.org/): used to parse command line arguments
- [rxi/log.c](https://github.com/rxi/log.c): used for logging

`barco` uses a number of LLVM-18-based tools for development, linting, formatting, debugging and Valgrind to check for memory leaks.

## Build

The included `Makefile` provides a few targets to build `barco`.
The variable `debug=1` can be set to run any of the targets in "debug" mode, which builds the project with debug symbols and without optimizations (especially useful for the debugger and valgrind).

```bash
# Build barco (executable is in bin/)
# The default target also runs, "make lint" and "make format" to lint and format the code
$ make


# Build barco with debug flags
$ make debug=1
```

## Development
`barco` is developed using [Visual Studio Code](https://code.visualstudio.com/) and [GitHub Codespaces](https://github.com/codespaces). The repository contains all the necessary configuration files to use these tools effectively.
`barco` relies on low-level Linux features, so it must be run on a Linux system. [GitHub Codespaces](https://github.com/codespaces) acts weird at times when tweaking low-level container settings: I found [getutm.app](https://getutm.app) to work well with [Debian](http://debian.org) on my Mac when in doubt.

The included `Makefile` provides a few targets useful for development:

```bash
# Run tests
$ make test

# Run linter
$ make lint

# Run formatter
$ make format

# Run valgrind
$ make check

# Clean the build
$ make clean
```

Furthermore, the project includes a [Visual Studio Code](https://code.visualstudio.com/) configuration in `.vscode/` that can be used to run the built-in debugger (at this moment it is "disabled" since `barco` should be run as `root` and [CodeLLDB](https://github.com/vadimcn/codelldb) does not have that option).

## Structure

The project is structured as follows:

```txt
├── .devcontainer       configuration for GitHub Codespaces
├── .github             configuration GitHub Actions and other GitHub features
├── .vscode             configuration for Visual Studio Code
├── bin                 the executable (created by make)
├── build               intermediate build files e.g. *.o (created by make)
├── docs                documentation
├── include             header files
├── lib                 third-party libraries
├── scripts             scripts for setup and other tasks
├── src                 C source files
│   ├── barco.c         (main)
│   └── *.c
├── tests               contains tests
├── .clang-format       configuration for clang-format
├── .cang-tidy          configuration for clang-tidy
├── .gitignore
├── .clang.cfg          configuration for the compiler
├── LICENSE
├── Makefile
└── README.md
```

## Testing and documentation

At the moment, the project does not contain any automated tests or tools to document the code.
In the future, suitable tools for automated testing and documentation might be added.

## Limitations

`barco` assumes that the host system is running a Linux kernel at version 6.0.x or higher and with user namespaces and cgroupsv2 enabled. The project has been tested on Debian 13.

`barco` does not handle network namespaces, so the container cannot access the network. Networking can roughly be setup as follows:

- create a new network namespace
- create a virtual ethernet pair
- move one end of the pair to the new network namespace
- assign an IP address to the interface in the new network namespace
- setup routing and NAT

In C this is usually done via the `rtnetlink` interface. Furthermore, network usage can be limited with the `net_prio` cgroup controller.

## Improvements

- Investigate further, document and refactor: user and mount and cgroup namespaces, syscalls and capabilities
- The functions in `cgroups.c`, `mount.c`, `sec.c`, `userns.c` are specific to `barco` and should be made more generic
- CMake and Conan are industry standards, so they should be used eventually instead of Make and the current build system. Unfortunately, CMake and Conan also add a lot of complexity which is not needed at this time.

## Credits

Some of the resources that have been used to develop `barco` are:

- [Linux kernel documentation](https://www.kernel.org/doc/html/latest/index.html)
- [user_namespaces docs](https://man7.org/linux/man-pages/man7/user_namespaces.7.html)
- [cgroup_namespaces docs](https://man7.org/linux/man-pages/man7/cgroup_namespaces.7.html)
- [mount_namespaces docs](https://man7.org/linux/man-pages/man7/mount_namespaces.7.html)
- [Linux containers in 500 lines of code](https://blog.lizzie.io/linux-containers-in-500-loc.html#fn.6)
- [Containers from scratch](https://medium.com/inside-sumup/containers-from-scratch-part-1-b719effd1e0a)
- [The current adoption status of cgroup v2 in containers](https://medium.com/nttlabs/cgroup-v2-596d035be4d7)
- [Docker under the Hood](https://medium.com/devops-dudes/docker-under-the-hood-0-naming-components-and-runtime-9a89cfbbe783)
- [A deep dive into Linux namespaces](https://ifeanyi.co/posts/linux-namespaces-part-1/)

## FAQ
- **Why C?** I haven't written much C since college and nostalgia got me.
- **What does "barco" mean?** It's [Venetian](https://vec.wikipedia.org/wiki/Barco) (my native language) for "hay barrack".
- **Nice logo, did you design it yourself?** Kind of, I asked Midjourney to come up with some designs.
