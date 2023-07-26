# <img src="./docs/barco.png" style="width:40px;padding-right:10px;margin-bottom:-8px;"> barco

Linux containers in C for educational purposes.

## Usage

The included `Makefile` provides a few targets to build, run, debug and inspect the program.

```bash
# Build the program
make
# Build the program with debug flags
make debug=1
...
# Setup build directory
make dir
# Lint the code
make lint
# Format the code
make format
# Run valgrind
make valgrind
# Run the lldb debugger
make debugger
# Run objdump
make objdump
# Clean the build
make clean
```

The variable `debug` can be set to `1` to run any of the targets in "debug" mode. This is especially useful for the debugger, valgrind and objdump.

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
