#pragma once

#include <stdint.h>

typedef struct flock_prng_s
{
    uint32_t state;
} flock_prng_t;

void flock_prng_seed(flock_prng_t *prng, uint32_t seed);
uint32_t flock_prng_get(flock_prng_t *prng);
