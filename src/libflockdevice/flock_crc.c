#include "flock_crc.h"

uint8_t flock_crc_dvb_s2(uint8_t crc, uint8_t b)
{
    crc ^= b;
    for (int ii = 0; ii < 8; ++ii)
    {
        if (crc & 0x80)
        {
            crc = (crc << 1) ^ 0xD5;
        }
        else
        {
            crc = crc << 1;
        }
    }
    return crc;
}
