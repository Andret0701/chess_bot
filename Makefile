CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3 -march=native -flto -funroll-loops -ffast-math -falign-functions=32 -fprefetch-loop-arrays -fomit-frame-pointer -DNDEBUG -Iengine
LDFLAGS = -flto
TARGET = main.exe
BUILD_DIR = build

# Collect all .c source files using a Makefile-compatible method
SRCS := $(filter-out $(BUILD_DIR)/%,$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) $(wildcard */*/*/*.c) $(wildcard */*/*/*/*.c))
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Default target
all: $(TARGET)

# Compile each source file into an object file in the build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir "$(dir $@)" 2>nul || exit 0
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files into the final executable with LTO
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Profiling build
profile: CFLAGS := -Wall -Wextra -std=c11 -pg -fno-omit-frame-pointer -g -Iengine
profile: LDFLAGS := -pg
profile: clean all


# Clean build directory and target
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del $(TARGET)

.PHONY: all clean profile
