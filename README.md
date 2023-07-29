# <img src="./docs/barco.png" style="width:40px;padding-right:10px;margin-bottom:-8px;"> barco

`barco` is a project I worked on to learn more about Linux containers and the Linux kernel, based on other guides on the internet.
`barco` enforces a minimal set of restrictions to run untrusted code, which is not recommended for production use, where a more robust solution should be used.
Some permissions however are absolutely not safe, and those are enforced by `barco`.

Linux containers are made up by a set of Linux kernel features:

- `namespaces`: are used to group kernel objects into different sets that can be accessed by specific process trees. There are different types
of `namespaces`, for example,the `PID` namespace is used to isolate the process tree, while the `network` namespace is used to isolate the network stack.
- `capabilities`: are used to set limits on what uid 0 (root) can do.
- `cgroups`: are used to limit the resources (e.g. memory, disk I/O, CPU-tme) that a process can use.
- `setrlimit`: similar to `cgroups` but older and offers more options.

Settings for `seccomp`, `capabilities` and `setrlimit` are handled via system calls. `cgroups` instead are handled via the `cgroupfs` virtual file system.
The scope of each of these features is somewhat unclear, and there is some overlap between them. The feature `user namespaces` should
help with this, but it is not available on all systems as it can be a security risk. `barco` tries to prevent nested `user namespaces` and only uses the feature if available.

## Usage

`barco` can be used to run `bin/sh` in the [busybox image](./data/img/busybox) as `root`:

```bash
$ sudo ./barco -m ./data/img/busybox -u 0 -c /bin/sh
```

## Setup
GitHub Codespaces is configured automatically with all the tools needed for developing and building the project.
In other environments (GitHub Actions, local...), the following script must be run before building the project to install the required tools:

```bash
# Install the tools
$ sudo ./scripts/install.sh
```

## Build

The included `Makefile` provides a few targets to build `barco`.
The variable `debug=1` can be set to run any of the targets in "debug" mode, which builds the project with the `-g` and `-O0` flags (the compiler includes debug symbols and disables optimizations for the executable and other objects).
The debug build is especially useful for the debugger, valgrind and objdump.

```bash
# Build the program
$ make

# Build the program with debug flags
$ make debug=1
```

Other targets are available to run a number of tools:

```bash
# Build third-party libraries
$ make libs

# Lint the code
$ make lint

# Format the code
$ make format

# Run valgrind
$ make valgrind

# Run the lldb debugger
$ make debugger

# Run objdump
$ make objdump

# Clean the build
$ make clean
```

## Dependencies
The project uses the following third-party libraries:

- `libseccomp-dev`: used to set up seccomp filters
- `libcap-dev`: used to set container capabilities
- [argtable](http://argtable.org/): used to parse command line arguments
- [rxi/log.c](https://github.com/rxi/log.c): used for logging

## Structure
The project is structured as follows:
```
├── .devcontainer       configuration for GitHub Codespaces
├── .github             configuration GitHub Actions and other GitHub features
├── .vscode             configuration for Visual Studio Code
├── bin                 the executable (created by make)
├── build               intermediate build files e.g. *.o (created by make)
├── data                data files e.g. container image
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
├── LICENSE
├── Makefile
└── README.md
```

## Tools

The project is developed using the following tools, which are installed by default on the Codespace.

- Visual Studio Code. The settings are included in the `.vscode` directory.
- `GitHub Codespaces` is used to develop the project.
- `make` is used to build the project.
- `clang` is the compiler of choice.
- `clangd` is used to provide code completion and navigation.
- `clang-tidy` is used to lint the code.
- `clang-format` is used to format the code.
- `valgrind` is used to check for memory leaks.
- `lldb` is used to debug the code.
- `objdump` is used to inspect the binary.

## Testing and documentation

At the moment, the project does not contain any automated tests or tools to document the code.
In the future, suitable tools for automated testing and documentation might be added.

## Limitations
barco does not handle network namespaces, so the container cannot access the network. Networking can roughly be setup as follows:
- create a new network namespace
- create a virtual ethernet pair
- move one end of the pair to the new network namespace
- assign an IP address to the interface in the new network namespace
- setup routing and NAT

In C this is usually done via the `rtnetlink` interface. Furthermore, network usage can be limited with the `net_prio` cgroup controller.

## Improvements

- [ ] Move print statements to main
- [ ] Use CLI library for parsing arguments
- [ ] Use CMake and Vcpkg
- [ ] Review code documentation
