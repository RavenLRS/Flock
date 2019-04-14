#include "flock_prng.h"

void flock_prng_seed(flock_prng_t *prng, uint32_t seed)
{
    prng->state = seed;
}

uint32_t flock_prng_get(flock_prng_t *prng)
{
    // Marsaglia Xorshift PRNG
    uint32_t state = prng->state;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    prng->state = state;
    return prng->state;
}
