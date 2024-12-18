CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -MMD -MP -O3
TARGET = main.exe
SRCS = main.c
BUILD_DIR = build
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS = $(SRCS:%.c=$(BUILD_DIR)/%.d)

all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Include dependencies
-include $(DEPS)

# Compile object files
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target for Windows
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del $(TARGET)

.PHONY: all clean
