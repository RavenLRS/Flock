#include "flock_platform.h"

void flock_millis_delay(unsigned delay_ms)
{
    unsigned end = flock_millis() + delay_ms;
    while (flock_millis() < end)
    {
    }
}
