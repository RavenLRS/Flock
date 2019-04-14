#pragma once

#include <stdint.h>

#define FLOCK_MHZ(mhz) (mhz * 1000000)

typedef enum
{
    FLOCK_BAND_433 = 0,
} flock_band_t;

uint64_t flock_band_get_default_freq(flock_band_t band);
