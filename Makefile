# -- Project Settings --
debug ?= 0
PWD := $(shell pwd)
SRC_DIR := $(PWD)/src
BUILD_DIR := $(PWD)/build
INCLUDE_DIR := $(PWD)/include
LIB_DIR := $(PWD)/lib
BIN_DIR := $(PWD)/bin
BIN := barco
BIN_ARGS_0 := -h
BIN_ARGS_1 := -u 0 -m . -c /bin/sh

# -- Barco Objects --
OBJS := $(BIN).o cgroups.o container.o

# -- Libraries Settings --
LIB_ARGTABLE_NAME := argtable3
LIB_ARGTABLE_PATH := $(LIB_DIR)/argtable/argtable3.c
LIB_LOG_NAME := log
LIB_LOG_PATH := $(LIB_DIR)/log.c/src/log.c
LIB_LOG_FLAGS := -DLOG_USE_COLOR
LIB_OBJS := $(BUILD_DIR)/$(LIB_ARGTABLE_NAME).o $(BUILD_DIR)/$(LIB_LOG_NAME).o

# -- Compiler Settings --
CC := clang-18 --config ./clang.cfg
LINTER := clang-tidy-18
FORMATTER := clang-format-18
DEBUGGER := lldb-18
DISASSEMBLER := llvm-objdump-18

# -- Debug Settings --
ifeq ($(debug), 1)
	CC := $(CC) -g -O0
else
	CC := $(CC) -Oz
endif

# -- Targets --

## -- Build barco executable --
$(BIN): format lint dir libs $(OBJS)
	$(CC) -o $(BIN_DIR)/$(BIN) $(foreach file,$(OBJS),$(BUILD_DIR)/$(file)) $(LIB_OBJS) $(CLIBS)

## -- Build object files --
%.o: dir $(SRC_DIR)/%.c
	@$(CC) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c

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
	@$(LINTER) --config-file=$(PWD)/.clang-tidy $(SRC_DIR)/* $(INCLUDE_DIR)/* --

## -- Run clang-format formatter  on src/ and lib/ directories --
format:
	@$(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/*

## -- Run valgrind memory checker on executable --
check: $(BIN)
	@valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BIN) $(BIN_ARGS_0)
	@valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BIN) $(BIN_ARGS_1)

## -- Run lldb debugger on executable --
debug: $(BIN)
	@$(DEBUGGER) $(BIN_DIR)/$(BIN)

## -- Run objdump disassembler on object files --
asm: dir $(BIN).o
	@$(DISASSEMBLER) -S -l -d $(BUILD_DIR)/$(BIN).o > $(BUILD_DIR)/$(BIN).o.asm

## -- Clean build and bin directories --
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: dir lint format check debug asm clean
