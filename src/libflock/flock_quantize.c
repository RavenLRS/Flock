#include <math.h>

#include "flock_quantize.h"

uint32_t flock_quantize_latf(float lat)
{
    return roundf(ldexpf(lat / 180.0f + 0.5f, 32));
}

uint32_t flock_quantize_lonf(float lon)
{
    return roundf(ldexpf(lon / 360.0f + 0.5f, 32));
}

float flock_dequantize_latf(uint32_t lat)
{
    return (lat / powf(2, 32)) * 180.0f - 90.0f;
}

float flock_dequantize_lonf(uint32_t lon)
{
    return (lon / powf(2, 32)) * 360.0f - 180.0f;
}

uint32_t flock_quantize_latd(double lat)
{
    return (ldexp((lat + 90.0) / 180.0, 32));
}

uint32_t flock_quantize_lond(double lon)
{
    return (ldexp((lon + 180.0) / 360.0, 32));
}

double flock_dequantize_latd(uint32_t lat)
{
    return (lat / pow(2, 32)) * 180.0 - 90.0;
}

double flock_dequantize_lond(uint32_t lon)
{
    return (lon / pow(2, 32)) * 360.0 - 180.0;
}

uint32_t _flock_quantize_cfp(int32_t c, uint32_t offset, float mult)
{
    uint32_t cc = c;
    cc += offset;
    return roundf(cc * mult);
}

int32_t _flock_dequantize_cfp(uint32_t c, uint32_t offset, float mult)
{
    uint32_t cc = roundf(c * mult);
    return cc - offset;
}

uint16_t flock_quantize_angle_radf(float rad)
{
    // Normalize to [0, 2*PI)
    while (rad < 0)
    {
        rad += 2 * (float)M_PI;
    }
    while (rad >= 2 * (float)M_PI)
    {
        rad -= 2 * (float)M_PI;
    }
    float mult = UINT16_MAX / (float)(2 * (float)M_PI);
    return roundf(rad * mult);
}

float flock_dequantize_angle_radf(uint16_t angle)
{
    float mult = (float)(2 * (float)M_PI) / UINT16_MAX;
    return angle * mult;
}

uint16_t _flock_quantize_angle_degfp(int16_t angle, int norm, float mult)
{
    while (angle < 0)
    {
        angle += norm;
    }
    while (angle >= norm)
    {
        angle -= norm;
    }
    return roundf(angle * mult);
}

int16_t _flock_dequantize_angle_degfp(uint16_t angle, float mult)
{
    return roundf(angle * mult);
}
