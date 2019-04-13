#include <cstdio>
#include <gtest/gtest.h>

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(x[0]))
#define TEST_LOG_F(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
