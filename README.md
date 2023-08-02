# <img src="./docs/barco.png" style="width:40px;padding-right:10px;margin-bottom:-8px;"> barco

`barco` is a project I worked on to learn more about Linux containers and the Linux kernel, based on other guides on the internet.
`barco` enforces a minimal set of restrictions to run untrusted code, which is not recommended for production use, where a more robust solution should be used.
Some permissions however are absolutely not safe, and those are enforced by `barco`.

Linux containers are made up by a set of Linux kernel features:

- `namespaces`: are used to group kernel objects into different sets that can be accessed by specific process trees. There are different types
of `namespaces`, for example,the `PID` namespace is used to isolate the process tree, while the `network` namespace is used to isolate the network stack.
- `capabilities`: are used to set limits on what uid 0 (root) can do.
- `cgroups`: are used to limit the resources (e.g. memory, disk I/O, CPU-tme) that a process can use.

Settings for `seccomp` and `capabilities` are handled via system calls, while `cgroups` are handled via the `cgroupfs` virtual file system. The scope of each of these features is somewhat unclear, and there is some overlap between them. The feature `user namespaces` should
help with this, but it is not available on all systems as it can be a security risk. `barco` tries to prevent nested `user namespaces` and only uses the feature if available.

## Usage

`barco` can be used to run `bin/sh` from the `/` directory as `root`:

```bash
$ sudo ./bin/barco -u 0 -m / -c /usr/bin/sh

23:21:12 INFO /erc/barco.c:90: initializing socket pair...
23:21:12 INFO /sre/barco.c:97: setting socket flags...
28:21:12 INFO /sre/barco.c:107: initializing container stack...
28:21:12 INFO /src/barco.c:116: initializing groups...
23:21:12 INFO /src/barco.c:126: initializing container...
28:21:12 INFO /src/barco.c:136: updating map...
28:21:12 INFO /src/barco.c:144: waiting for container to exit...
23:21:12 INFO /src/container.c:360: ### BARCONTAINER STARTING - type 'exit' to quit ###

# ls
bin         home                lib32       media       root        sys         vmlinuz
boot        initrd.img          lib64       mnt         run         tmp         vmlinuz.old
dev         initrd.img.old      libx32      opt         sbin        usr
etc         lib                 lost+found  proc        srv         var
# echo "i am in a container"
i am in a container
# exit

23:24:23 INFO /src/barco.c:150: cleaning up...
28:24:23 INFO /sre/barco.c:164: so long and thanks for all the fish
```

## Setup

`barco` requires a number of tools and libraries to be installed to build the project and for development.
`barco` relies on low-level Linux features, so it must be run on a Linux system. I found [getutm.app](https://getutm.app) to work well with [Debian](http://debian.org) on my Mac.

```bash
# Install required tooling
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

`barco` uses the following LLVM-18-based tools for development:

- `clang` is the compiler of choice.
- `clangd` is used to provide code completion and navigation.
- `clang-tidy` is used to lint the code.
- `clang-format` is used to format the code.
- `valgrind` is used to check for memory leaks.

## Build

The included `Makefile` provides a few targets to build `barco`.
The variable `debug=1` can be set to run any of the targets in "debug" mode, which builds the project with the with debug symbols and without optimizations.
The debug build is especially useful for the debugger and valgrind.

```bash
# Build barco (executable is in bin/)
# The default target also runs, "make lint" and "make format" to lint and format the code
$ make


# Build barco with debug flags
$ make debug=1
```

## Development
The included `Makefile` provides a few targets useful for development:

```bash
# Run tests
$ make test

# Run valgrind
$ make check

# Clean the build
$ make clean
```

Furthermore, the project includes a [Visual Studio Code](https://code.visualstudio.com/) configuration in `.vscode/` that can be used to run the built-in debugger.

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

`barco` does not handle network namespaces, so the container cannot access the network. Networking can roughly be setup as follows:

- create a new network namespace
- create a virtual ethernet pair
- move one end of the pair to the new network namespace
- assign an IP address to the interface in the new network namespace
- setup routing and NAT

In C this is usually done via the `rtnetlink` interface. Furthermore, network usage can be limited with the `net_prio` cgroup controller.

## Improvements

- CMake and Conan are industry standards, so they should be used eventually instead of Make and the current build system. Unfortunately, CMake and Conan also add a lot of complexity which is not needed at this time.

## Todo

- Review logging location and verbosity to make sure they are correct, explanatory, useful and consistent
- Review comments to make sure they are correct, explanatory, useful and consistent
- General refactoring / code splitting
- Pass arg to container command
- Add support for cgroupsv2
