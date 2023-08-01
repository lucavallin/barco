# Project Settings
debug ?= 0
SRC_DIR := ./src
BUILD_DIR := ./build
INCLUDE_DIR := ./include
LIB_DIR := ./lib
TESTS_DIR := ./tests
BIN_DIR := ./bin

# Executable settings
BARCO := barco
BARCO_ARGS_0 := --help
BARCO_ARGS_1 := -u 0 -m . -c /bin/sh

# Libraries settings
LIB_ARGTABLE_NAME := argtable3
LIB_ARGTABLE_PATH := $(LIB_DIR)/argtable/argtable3.c
LIB_LOG_NAME := log
LIB_LOG_PATH := $(LIB_DIR)/log/src/log.c
LIB_LOG_FLAGS := -DLOG_USE_COLOR

# Barco object files
OBJS := $(BARCO).o cgroups.o container.o argtable3.o log.o

# Compiler settings
CC := clang-18 --config ./clang.cfg
LINTER := clang-tidy-18
FORMATTER := clang-format-18
DEBUGGER := lldb-18
DISASSEMBLER := llvm-objdump-18

# Debug Settings
ifeq ($(debug), 1)
	CC := $(CC) -g -O0
else
	CC := $(CC) -Oz
endif

# Targets

# Build barco executable
$(BARCO): format lint dir $(OBJS)
	$(CC) -o $(BIN_DIR)/$(BARCO) $(foreach file,$(OBJS),$(BUILD_DIR)/$(file))

# Build object files
%.o: dir $(SRC_DIR)/%.c
	@$(CC) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c
# Build third-party libraries
$(LIB_ARGTABLE_NAME).o: dir $(LIB_ARGTABLE_PATH)
	@$(CC) -o $(BUILD_DIR)/$(LIB_ARGTABLE_NAME).o -c $(LIB_ARGTABLE_PATH)
$(LIB_LOG_NAME).o: dir $(LIB_LOG_PATH)
	@$(CC) -o $(BUILD_DIR)/$(LIB_LOG_NAME).o -c $(LIB_LOG_PATH) $(LIB_LOG_FLAGS)

# Run CUnit tests
test: dir
	@$(CC) -lcunit -o $(BIN_DIR)/$(BARCO)_test $(TESTS_DIR)/$(BARCO)_test.c
	@$(BIN_DIR)/$(BARCO)_test

# Run linter on source directories
lint:
	@$(LINTER) --config-file=./.clang-tidy $(SRC_DIR)/* $(INCLUDE_DIR)/* $(TESTS_DIR)/* --

# Run formatter on source directories
format:
	@$(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/*  $(TESTS_DIR)/*

# Run valgrind memory checker on executable
check: $(BARCO)
	@valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BARCO) $(BARCO_ARGS_0)
	@valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BARCO) $(BARCO_ARGS_1)

# Run lldb debugger on executable
debug: $(BARCO)
	@$(DEBUGGER) $(BIN_DIR)/$(BARCO)

# Run objdump disassembler on object files
asm: dir $(BARCO).o
	@$(DISASSEMBLER) -S -l -d $(BUILD_DIR)/$(BARCO).o > $(BUILD_DIR)/$(BARCO).o.asm

# Setup dependencies for build and development
setup:
	# Update apt and upgrade packages
	sudo apt update
	sudo apt upgrade -y

	# Install OS dependencies
	sudo apt install -y bash libarchive-tools lsb-release wget software-properties-common gnupg

	# Install LLVM tools required for building the project
	wget https://apt.llvm.org/llvm.sh
	chmod +x llvm.sh
	sudo ./llvm.sh 18
	rm llvm.sh

	# Install Clang development tools
	sudo apt install -y clang-format-18 clang-tidy-18 clang-tools clangd valgrind

	# Install C dependencies
	sudo apt install -y libseccomp-dev libcap-dev libbsd-dev

	# Install CUnit testing framework
	sudo apt install -y libcunit1 libcunit1-doc libcunit1-dev

	# Cleanup
	sudo apt autoremove -y

# Setup build and bin directories
dir:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Clean build and bin directories
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: test lint format check debug asm setup dir clean
