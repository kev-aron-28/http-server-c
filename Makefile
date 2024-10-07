# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/webserver

# Source and header files
SRCS = $(SRC_DIR)/webserver.c
HDRS = $(INCLUDE_DIR)/webserver.h

# Default target: create the executable directly
all: $(TARGET)

# Compile and link the webserver
$(TARGET): $(SRCS) $(HDRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

# Clean up the build (remove the bin directory)
clean:
	rm -rf $(BIN_DIR)

# Phony targets to avoid conflict with files named "all" or "clean"
.PHONY: all clean
