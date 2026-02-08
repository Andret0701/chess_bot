#pragma once

#include <stdio.h>

#define TEST_MESSAGE_SIZE 512
#define TEST_DOUBLE_PRECISION 1e-12

#define ASSERT_DOUBLE_EQUALS(expected, actual, fmt, ...)                                       \
    do                                                                                         \
    {                                                                                          \
        char msg[TEST_MESSAGE_SIZE];                                                           \
        (void)snprintf(msg, sizeof(msg), (fmt), __VA_ARGS__);                                  \
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE((TEST_DOUBLE_PRECISION), (expected), (actual), msg); \
    } while (0)
