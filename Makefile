# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3 -funroll-loops -ffast-math -falign-functions=32 -fprefetch-loop-arrays -fomit-frame-pointer -DNDEBUG -Iengine
PROFILE_FLAGS = -pg -no-pie -fno-builtin -O3 -fno-inline
TARGET = andobot.exe
BUILD_DIR = build
UNITY_SRC = $(BUILD_DIR)/unity.c

# Include all .c files in all directories except the build directory
SRCS = $(shell dir /s /b *.c | findstr /v /i /c:"\\$(BUILD_DIR)\\")

# Default build
all: $(UNITY_SRC)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@echo /* Unity Build File */ > $(UNITY_SRC)
	@for %%i in ($(SRCS)) do @echo #include "%%i" >> $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(UNITY_SRC)

# Profile build
profile: CFLAGS = $(PROFILE_FLAGS)
profile: clean all

# Unity source file dependency
$(UNITY_SRC): $(SRCS)

# Clean build artifacts
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del $(TARGET)

# Ensure these targets are not treated as files
.PHONY: all clean profile
