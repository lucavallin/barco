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
BARCO_ARGS_1 := -u 0 -m / -c "/bin/bash -c" -a exit
BARCO_ARGS_2 := -u 0 -m / -c "/bin/bash -c" -a exit -v

# Libraries settings
LIB_ARGTABLE_REPO := https://github.com/argtable/argtable3/releases/download/v3.2.2.f25c624/argtable-v3.2.2.f25c624-amalgamation.tar.gz
LIB_ARGTABLE_NAME := argtable3
LIB_ARGTABLE_DIR := $(LIB_DIR)/argtable
LIB_ARGTABLE_SRC := $(LIB_ARGTABLE_DIR)/argtable3.c
LIB_LOG_REPO := https://github.com/rxi/log.c/archive/refs/heads/master.zip
LIB_LOG_NAME := log
LIB_LOG_DIR := $(LIB_DIR)/log
LIB_LOG_SRC := $(LIB_LOG_DIR)/log.c
LIB_LOG_FLAGS := -DLOG_USE_COLOR

# Barco object files
OBJS := $(BARCO).o cgroups.o container.o mount.o sec.o user.o $(LIB_ARGTABLE_NAME).o $(LIB_LOG_NAME).o

# Compiler settings
CC := clang-18
LINTER := clang-tidy-18
FORMATTER := clang-format-18
DEBUGGER := lldb-18
DISASSEMBLER := llvm-objdump-18

# Compiler and Linker flags Settings:
# 	-std=gnu17: Use the GNU17 standard
# 	-D _GNU_SOURCE: Use GNU extensions
# 	-D __STDC_WANT_LIB_EXT1__: Use C11 extensions
# 	-Wall: Enable all warnings
# 	-Wextra: Enable extra warnings
# 	-pedantic: Enable pedantic warnings
# 	-I$(INCLUDE_DIR): Include the include directory
# 	-I$(LIB_ARGTABLE_DIR): Include the argtable library directory
# 	-I$(LIB_LOG_DIR): Include the log library directory
# 	-lcap: Link to libcap
# 	-lseccomp: Link to libseccomp
# 	-lm: Link to libm
CFLAGS := -std=gnu17 -D _GNU_SOURCE -D __STDC_WANT_LIB_EXT1__ -Wall -Wextra -pedantic -I$(INCLUDE_DIR) -I$(LIB_ARGTABLE_DIR) -I$(LIB_LOG_DIR)
LFLAGS := -lcap -lseccomp -lm

ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -Oz
endif

# Targets

# Build barco executable
$(BARCO): format lint dir $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(BIN_DIR)/$(BARCO) $(foreach file,$(OBJS),$(BUILD_DIR)/$(file))

# Build object files
%.o: dir $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c
# Build third-party libraries
$(LIB_ARGTABLE_NAME).o: dir $(LIB_ARGTABLE_SRC)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(LIB_ARGTABLE_NAME).o -c $(LIB_ARGTABLE_SRC)
$(LIB_LOG_NAME).o: dir $(LIB_ARGTABLE_SRC)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(LIB_LOG_NAME).o -c $(LIB_LOG_SRC) $(LIB_LOG_FLAGS)

# Run CUnit tests
test: dir
	@$(CC) $(CFLAGS) -lcunit -o $(BIN_DIR)/$(BARCO)_test $(TESTS_DIR)/$(BARCO)_test.c
	@$(BIN_DIR)/$(BARCO)_test

# Run linter on source directories
lint:
	@$(LINTER) --config-file=.clang-tidy $(SRC_DIR)/* $(INCLUDE_DIR)/* $(TESTS_DIR)/* -- $(CFLAGS)

# Run formatter on source directories
format:
	@$(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/* $(TESTS_DIR)/*

# Run valgrind memory checker on executable
check: $(BARCO)
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BARCO) $(BARCO_ARGS_0)
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BARCO) $(BARCO_ARGS_1)
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BARCO) $(BARCO_ARGS_2)

# Setup dependencies for build and development
setup:
	# Update apt and upgrade packages
	@sudo apt update
	@sudo DEBIAN_FRONTEND=noninteractive apt upgrade -y

	# Install OS dependencies
	@sudo apt install -y bash libarchive-tools lsb-release wget software-properties-common gnupg

	# Install LLVM tools required for building the project
	@wget https://apt.llvm.org/llvm.sh
	@chmod +x llvm.sh
	@sudo ./llvm.sh 18
	@rm llvm.sh

	# Install Clang development tools
	@sudo apt install -y clang-format-18 clang-tidy-18 clang-tools clangd valgrind

	# Install non-standard system libraries
	@sudo apt install -y libseccomp-dev libcap-dev

	# Install CUnit testing framework
	@sudo apt install -y libcunit1 libcunit1-doc libcunit1-dev

	# Install third-party libraries and structure them
	@mkdir -p $(LIB_DIR)/argtable $(LIB_DIR)/log
	@echo "Installing argtable..."
	@wget -qO- $(LIB_ARGTABLE_REPO) | bsdtar -xvf- --strip=1 -C $(LIB_DIR)/argtable *.c *.h 2> /dev/null
	@find $(LIB_DIR)/argtable/* -d -type d -exec rm -rf '{}' \; 2> /dev/null
	@echo "Installing log..."
	@wget -qO- $(LIB_LOG_REPO) | bsdtar -xvf- --strip=2 -C $(LIB_DIR)/log *.c *.h 2> /dev/null

	# Cleanup
	@sudo apt autoremove -y

# Setup build and bin directories
dir:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Clean build and bin directories
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: lint format check setup dir clean deps
