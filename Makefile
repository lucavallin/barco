# -- Project Settings --
debug ?= 0
SRC_DIR := ./src
BUILD_DIR := ./build
BIN := barco
OBJS := $(BIN).o

# -- Compiler Settings --
CC := clang
C_VERSION := c18
C_VERSION_FLAG := -std=$(C_VERSION)
CFLAGS := $(C_VERSION_FLAG) -Wall -Wextra -pedantic -pedantic-errors

# -- Debug Settings --
ifeq ($(debug), 1)
	BUILD_DIR := $(BUILD_DIR)/debug
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -O2
endif

# -- Targets --
all: clean $(BIN)

$(BIN): dir $(OBJS)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(BIN) $(foreach file,$(OBJS),$(BUILD_DIR)/$(file))

%.o: dir $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c

# -- Utility targets --
dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

lint:
	@find $(SRC_DIR) -regex '.*\.\(c\|h\)' -exec clang-tidy --header-filter=.* --extra-arg=$(C_VERSION_FLAG) {} \;

format:
	@find $(SRC_DIR) -regex '.*\.\(c\|h\)' -exec clang-format -style=file -i {} \;

valgrind: $(BIN)
	@valgrind --leak-check=full --show-leak-kinds=all $(BUILD_DIR)/$(BIN)

debugger: $(BIN)
	@lldb $(BUILD_DIR)/$(BIN)

objdump: dir $(BIN).o
	@objdump -S -l -d $(BUILD_DIR)/$(BIN).o > $(BUILD_DIR)/$(BIN).o.asm

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: lint format valgrind debugger objdump clean
