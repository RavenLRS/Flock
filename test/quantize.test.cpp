#include <cmath>

#include "test.h"

#include "quantize.h"

#define QUANTIZE_DOUBLE_MAX_ERROR 1e-7 // < 11.132 mm

#define QUANTIZE_FLOAT_MAX_ENCODE_ERROR 0xff
#define QUANTIZE_FLOAT_MAX_ERROR 2e-5 // < 2 * 1.1132 m

#define QUANTIZE_FP_EXP 7
#define QUANTIZE_FP_MAX_ERROR 2e2 // < 2 * 1.1132 m

#define QUANTIZE_ANGLE_MAX_ERROR 1e-3
#define QUANTIZE_ANGLE_FP_EXP 1
#define QUANTiZE_ANGLE_FP_MAX_ERROR 2

typedef struct quantize_coord_case_s
{
    double c;
    uint32_t q;
} quantize_coord_case_t;

typedef struct quantize_angle_case_s
{
    float a;
    uint16_t q;
} quantize_angle_case_t;

quantize_coord_case_t latitudes[] = {
    {27.988056, 0xa7ce23e4},
    {0, 0x80000000},
    {-89, 0x16c16c1},
    {89, 0xfe93e93e},
};

quantize_coord_case_t longitudes[] = {
    {86.925278, 0xbdd04391},
    {0, 0x80000000},
    {-179, 0xb60b60},
    {179, 0xff49f49f},
};

quantize_angle_case_t angles[] = {
    {M_PI, 0x8000},
    {-M_PI, 0x8000},
    {0, 0},
    {2 * M_PI, 0},
};

TEST(Quantize, Double)
{
    double err = 0;
    for (int ii = 0; ii < ARRAY_COUNT(latitudes); ii++)
    {
        uint32_t q = flock_quantize_latd(latitudes[ii].c);
        EXPECT_EQ(q, latitudes[ii].q);
        double c = flock_dequantize_latd(q);
        err += abs(c - latitudes[ii].c);
        EXPECT_NEAR(c, latitudes[ii].c, QUANTIZE_DOUBLE_MAX_ERROR);
    }
    for (int ii = 0; ii < ARRAY_COUNT(longitudes); ii++)
    {
        uint32_t q = flock_quantize_lond(longitudes[ii].c);
        EXPECT_EQ(q, longitudes[ii].q);
        double c = flock_dequantize_lond(q);
        err += abs(c - longitudes[ii].c);
        EXPECT_NEAR(c, longitudes[ii].c, QUANTIZE_DOUBLE_MAX_ERROR);
    }
    TEST_LOG_F("Avg. Err. (double) %.32f\n", err / (ARRAY_COUNT(latitudes) + ARRAY_COUNT(longitudes)));
}

TEST(Quantize, Float)
{
    float err = 0;
    for (int ii = 0; ii < ARRAY_COUNT(latitudes); ii++)
    {
        uint32_t q = flock_quantize_latf(latitudes[ii].c);
        EXPECT_NEAR(q, latitudes[ii].q, QUANTIZE_FLOAT_MAX_ENCODE_ERROR);
        float c = flock_dequantize_latf(q);
        err += fabs(c - latitudes[ii].c);
        EXPECT_NEAR(c, latitudes[ii].c, QUANTIZE_FLOAT_MAX_ERROR);
    }
    for (int ii = 0; ii < ARRAY_COUNT(longitudes); ii++)
    {
        uint32_t q = flock_quantize_lonf(longitudes[ii].c);
        EXPECT_NEAR(q, longitudes[ii].q, QUANTIZE_FLOAT_MAX_ENCODE_ERROR);
        float c = flock_dequantize_lonf(q);
        err += fabs(c - longitudes[ii].c);
        EXPECT_NEAR(c, longitudes[ii].c, QUANTIZE_FLOAT_MAX_ERROR);
    }
    TEST_LOG_F("Avg. Err. (float) %.32f\n", err / (ARRAY_COUNT(latitudes) + ARRAY_COUNT(longitudes)));
}

static float normalize_angle(float a)
{
    while (a < 0)
    {
        a += 2 * M_PI;
    }
    while (a >= 2 * M_PI)
    {
        a -= 2 * M_PI;
    }
    return a;
}

TEST(Quantize, FixedPoint)
{
    int32_t err = 0;
    for (int ii = 0; ii < ARRAY_COUNT(latitudes); ii++)
    {
        int32_t cfp = latitudes[ii].c * pow(10, QUANTIZE_FP_EXP);
        uint32_t q = flock_quantize_latfp(cfp, QUANTIZE_FP_EXP);
        EXPECT_NEAR(q, latitudes[ii].q, QUANTIZE_FLOAT_MAX_ENCODE_ERROR);
        int32_t dcfp = flock_dequantize_latfp(q, QUANTIZE_FP_EXP);
        err += abs(cfp - dcfp);
        EXPECT_NEAR(cfp, dcfp, QUANTIZE_FP_MAX_ERROR);
    }
    for (int ii = 0; ii < ARRAY_COUNT(longitudes); ii++)
    {
        int32_t cfp = longitudes[ii].c * pow(10, QUANTIZE_FP_EXP);
        uint32_t q = flock_quantize_lonfp(cfp, QUANTIZE_FP_EXP);
        EXPECT_NEAR(q, longitudes[ii].q, QUANTIZE_FLOAT_MAX_ENCODE_ERROR);
        int32_t dcfp = flock_dequantize_lonfp(q, QUANTIZE_FP_EXP);
        err += abs(cfp - dcfp);
        EXPECT_NEAR(cfp, dcfp, QUANTIZE_FP_MAX_ERROR);
    }
    TEST_LOG_F("Avg. Err. (fp) %.32f\n", (err / pow(10, QUANTIZE_FP_EXP)) / (ARRAY_COUNT(latitudes) + ARRAY_COUNT(longitudes)));
}

TEST(Quantize, AngleRadians)
{
    for (int ii = 0; ii < ARRAY_COUNT(angles); ii++)
    {
        uint16_t q = flock_quantize_angle_radf(angles[ii].a);
        EXPECT_EQ(q, angles[ii].q);
        float da = flock_dequantize_angle_radf(q);
        float expected = normalize_angle(angles[ii].a);
        EXPECT_NEAR(da, expected, QUANTIZE_ANGLE_MAX_ERROR);
    }
}

float rad2deg(float r)
{
    return r * (180.0 / M_PI);
}

int16_t rad2degfp(float r)
{
    return rad2deg(r) * pow(10, QUANTIZE_ANGLE_FP_EXP);
}

TEST(Quantize, AngleDegrees)
{
    for (int ii = 0; ii < ARRAY_COUNT(angles); ii++)
    {
        int16_t a = rad2degfp(angles[ii].a);
        uint16_t q = flock_quantize_angle_degfp(a, QUANTIZE_ANGLE_FP_EXP);
        EXPECT_EQ(q, angles[ii].q);
        int16_t da = flock_dequantize_angle_degfp(q, QUANTIZE_ANGLE_FP_EXP);
        int16_t expected = rad2degfp(normalize_angle(angles[ii].a));
        EXPECT_NEAR(da, expected, QUANTiZE_ANGLE_FP_MAX_ERROR);
    }
}
