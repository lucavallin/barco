# -- Project Settings --
debug ?= 0
SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin
INCLUDE_DIR := ./include
LIB_DIR := ./lib
BIN := barco
OBJS := $(BIN).o cgroup.o container.o namespace.o

# -- Libraries Settings --
LIB_ARGTABLE_NAME := argtable3
LIB_ARGTABLE_PATH := $(LIB_DIR)/argtable/argtable3.c
LIB_LOG_NAME := log
LIB_LOG_PATH := $(LIB_DIR)/log.c/src/log.c
LIB_LOG_FLAGS := -DLOG_USE_COLOR
LIB_OBJS := $(BUILD_DIR)/$(LIB_ARGTABLE_NAME).o $(BUILD_DIR)/$(LIB_LOG_NAME).o

# -- Compiler Settings --
CC := clang
CLIBS := -lcap -lseccomp -lm
CFLAGS := -std=gnu17 -Wall -Wextra -pedantic -pedantic-errors

# -- Debug Settings --
ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -O2
endif

# -- Targets --

## -- Build barco executable --
$(BIN): dir libs $(OBJS)
	@$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN) $(foreach file,$(OBJS),$(BUILD_DIR)/$(file)) $(LIB_OBJS) $(CLIBS)

## -- Build object files --
%.o: dir $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c

## -- Build third-party libraries --
libs: dir
	@$(CC) -o $(BUILD_DIR)/$(LIB_ARGTABLE_NAME).o -c $(LIB_ARGTABLE_PATH)
	@$(CC) -o $(BUILD_DIR)/$(LIB_LOG_NAME).o -c $(LIB_LOG_PATH) $(LIB_LOG_FLAGS)

# -- Utility targets --

## -- Setup build and bin directories --
dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

## -- Run clang-tidy linter on src/ and lib/ directories --
lint:
	@find $(SRC_DIR) $(INCLUDE_DIR) -regex '.*\.\(c\|h\)' -exec clang-tidy --header-filter=.* {} \;

## -- Run clang-format formatter  on src/ and lib/ directories --
format:
	@find $(SRC_DIR) $(INCLUDE_DIR) -regex '.*\.\(c\|h\)' -exec clang-format -style=file -i {} \;

## -- Run valgrind memory checker on executable --
valgrind: $(BIN)
	@valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BIN)

## -- Run lldb debugger on executable --
debugger: $(BIN)
	@lldb $(BIN_DIR)/$(BIN)

## -- Run objdump disassembler on object files --
objdump: dir $(BIN).o
	@objdump -S -l -d $(BIN_DIR)/$(BIN).o > $(BIN_DIR)/$(BIN).o.asm

## -- Clean build and bin directories --
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: dir lint format valgrind debugger objdump clean
