#include "flock_bands.h"

uint64_t flock_band_get_default_freq(flock_band_t band)
{
    switch (band)
    {
    case FLOCK_BAND_433:
        return FLOCK_MHZ(433);
    }
    return 0;
}
