#pragma once

#include <assert.h>

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(x[0]))
#define BREAKPOINT __asm("bkpt 255")
#define UNREACHABLE() \
    do                \
    {                 \
        BREAKPOINT;   \
    } while (1)

#ifndef MAX
#define MAX(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#ifndef MIN
#define MIN(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })
#endif

#define CONSTRAIN(v, min, max) ({ \
    __typeof(v) __v = (v);        \
    __typeof(min) __min = (min);  \
    __typeof(max) __max = (max);  \
    assert(__min < __max);        \
    if (__v < __min)              \
    {                             \
        __v = __min;              \
    }                             \
    else if (__v > __max)         \
    {                             \
        __v = __max;              \
    }                             \
    __v;                          \
})

#define CONSTRAIN_TO_I8(x) CONSTRAIN(x, INT8_MIN, INT8_MAX)
