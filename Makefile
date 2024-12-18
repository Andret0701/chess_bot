CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3 -march=native -flto -funroll-loops -Iengine
TARGET = main.exe
BUILD_DIR = build
UNITY_SRC = $(BUILD_DIR)/unity.c

# Include all .c files in all directories except the build directory
SRCS = $(shell dir /s /b *.c | findstr /v /i /c:"\\$(BUILD_DIR)\\")

all: $(UNITY_SRC)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@echo /* Unity Build File */ > $(UNITY_SRC)
	@for %%i in ($(SRCS)) do @echo #include "%%i" >> $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(UNITY_SRC)

$(UNITY_SRC): $(SRCS)

# Clean target for Windows
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del $(TARGET)

.PHONY: all clean
