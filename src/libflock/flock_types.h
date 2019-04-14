#pragma once

#include <stdint.h>

#define FLOCK_ADDR_SIZE 6

typedef enum
{
    FLOCK_RADIO_TYPE_LORA_SUB_GHZ = 1,
    FLOCK_RADIO_TYPE_LORA_2G4 = 2,
} flock_radio_type_t;

typedef enum
{
    FLOCK_BIRD_TYPE_UNKNOWN = 0,
    FLOCK_BIRD_TYPE_AIRPLANE = 1,
    FLOCK_BIRD_TYPE_BOAT = 2,
    FLOCK_BIRD_TYPE_FLYING_WING = 3,
    FLOCK_BIRD_TYPE_HELICOPTER = 4,
    FLOCK_BIRD_TYPE_MULTIROTOR = 5,
    FLOCK_BIRD_TYPE_ROVER = 6,
} flock_bird_type_t;

typedef enum
{
    FLOCK_BIRD_CAN_NOTIFY_PILOT = 1 << 0,
    FLOCK_BIRD_HAS_AUTOPILOT = 1 << 2,
    FLOCK_BIRD_HAS_COLLISION_AVOIDANCE = 1 << 3,
    FLOCK_BIRD_IS_MANNED = 1 << 4,
} flock_bird_flags_t;

typedef struct flock_addr_s
{
    uint8_t data[FLOCK_ADDR_SIZE];
} __attribute__((packed)) flock_addr_t;

typedef struct flock_dev_info_s
{
    uint8_t flock_version;
    char device_name[12];
    uint8_t device_version[3];
    flock_addr_t device_addr;
    uint16_t host_posvel_interval_ms;
    uint8_t device_radio_type;
    uint64_t device_radio_min_freq;
    uint64_t device_radio_max_freq;
    uint64_t device_radio_default_freq;
} __attribute__((packed)) flock_dev_info_t;

typedef struct flock_bird_info_s
{
    uint8_t type;   // from flock_bird_type_t
    uint16_t flags; // from flock_bird_flags_t
    char name[17];
} __attribute__((packed)) flock_bird_info_t;

typedef struct flock_posvel_s
{
    int32_t latitude;
    int32_t longitude;
    uint16_t altitude;
    uint16_t groundSpeed;
    int16_t verticalSpeed;
    uint16_t heading;
} __attribute__((packed)) flock_posvel_t;

typedef struct flock_remote_bird_info_s
{
    flock_addr_t from;
    flock_bird_info_t info;
} __attribute__((packed)) flock_remote_bird_info_t;

typedef struct flock_remote_posvel_s
{
    flock_addr_t from;
    flock_posvel_t posvel;
} __attribute__((packed)) flock_remote_posvel_t;
