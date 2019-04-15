#pragma once

#include <stdint.h>

#include "flock_types.h"

typedef enum
{
    FLOCK_AIR_FRAME_TYPE_ADVERTISE = 0,
    FLOCK_AIR_FRAME_TYPE_BIRD_INFO = 1,
    FLOCK_AIR_FRAME_TYPE_POSVEL = 2,
    FLOCK_AIR_FRAME_TYPE_DATA_BROADCAST = 3,
} flock_air_frame_type_t;

// up to 16 types of frames leaves us some room for
// future expansion
#define FLOCK_FRAME_TYPE_BITS 4

// Up to 128 nodes on a flock
#define FLOCK_AIR_ADDR_BITS 7

typedef uint8_t flock_air_addr_t;

typedef struct flock_air_header_s
{
    flock_air_frame_type_t type : FLOCK_FRAME_TYPE_BITS;
    flock_air_addr_t air_addr : FLOCK_AIR_ADDR_BITS;
} __attribute__((packed)) flock_air_header_t;

typedef struct flock_air_advertise_addr_s
{
    flock_air_header_t hdr;
} __attribute__((packed)) flock_air_advertise_addr_t;

typedef struct flock_air_bird_info_s
{
    flock_air_header_t hdr;
    flock_bird_info_t info;
    // Including the addr here lets us detect collisions when 2
    // disjointed flocks merge.
    flock_addr_t addr;
} __attribute__((packed)) flock_air_bird_info_t;

typedef struct flock_air_posvel_s
{
    flock_air_header_t hdr;
    uint16_t latitude;
    uint16_t longitude;
    uint16_t altitude : 13; // m in +1000 offset, range [-1000, 7192]
    uint8_t groundSpeed;    // m/s in 0.3 steps, range [0, 76.5]
    uint8_t verticalSpeed;  // m/s in 0.1 steps, range [-12.8, 12.7]
    uint8_t heading;        // [0, 2*pi) quantized to uint8_t

} __attribute__((packed)) flock_air_bird_info_t;
