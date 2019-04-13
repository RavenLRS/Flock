#pragma once

#include <stdint.h>

// Quantize/dequantize latitude and longitude
// as floats
uint32_t flock_quantize_latf(float lat);
uint32_t flock_quantize_lonf(float lon);
float flock_dequantize_latf(uint32_t lat);
float flock_dequantize_lonf(uint32_t lon);

// Quantize/dequantize latitude and longitude
// as double.
uint32_t flock_quantize_latd(double lat);
uint32_t flock_quantize_lond(double lon);
double flock_dequantize_latd(uint32_t lat);
double flock_dequantize_lond(uint32_t lon);

// Quantize/dequantize latitude and longitude
// as fixed point. For example, when using
// deg * 10`000`000 as a unit, e is 7.

#define _f10exp(x, _e) (x * 1e##_e)
#define _ffp_lat_o(_e) _f10exp(90, _e)
#define _ffp_lat_m(_e) (INT32_MAX / (float)_ffp_lat_o(_e))
#define _ffp_lat_im(_e) ((float)_ffp_lat_o(_e) / INT32_MAX)
#define _ffp_lon_o(_e) _f10exp(180, _e)
#define _ffp_lon_m(_e) (INT32_MAX / (float)_ffp_lon_o(_e))
#define _ffp_lon_im(_e) ((float)_ffp_lon_o(_e) / INT32_MAX)

uint32_t _flock_quantize_cfp(int32_t c, uint32_t offset, float mult);
int32_t _flock_dequantize_cfp(uint32_t c, uint32_t offset, float mult);

#define flock_quantize_latfp(lat, e) _flock_quantize_cfp(lat, _ffp_lat_o(e), _ffp_lat_m(e))
#define flock_quantize_lonfp(lon, e) _flock_quantize_cfp(lon, _ffp_lon_o(e), _ffp_lon_m(e))
#define flock_dequantize_latfp(lat, e) _flock_dequantize_cfp(lat, _ffp_lat_o(e), _ffp_lat_im(e))
#define flock_dequantize_lonfp(lon, e) _flock_dequantize_cfp(lon, _ffp_lon_o(e), _ffp_lon_im(e))

// Quantize/dequantize an angle in radians as a float
// Note that the quantization takes any angle and normalizes
// it, while dequantization always returns an angle
// normalized to [0, 2*PI).
uint16_t flock_quantize_angle_radf(float rad);
float flock_dequantize_angle_radf(uint16_t angle);

// Quantize/dequantize an angle in degrees as a fixed point
// number. Same normalizations caveats as the radian based
// version apply.
uint16_t _flock_quantize_angle_degfp(int16_t angle, int norm, float mult);
int16_t _flock_dequantize_angle_degfp(uint16_t angle, float mult);

#define flock_quantize_angle_degfp(angle, e) _flock_quantize_angle_degfp(angle, _f10exp(360, e), UINT16_MAX / (float)_f10exp(360, e))
#define flock_dequantize_angle_degfp(angle, e) _flock_dequantize_angle_degfp(angle, (float)_f10exp(360, e) / UINT16_MAX)
