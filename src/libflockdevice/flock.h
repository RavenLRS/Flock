#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <libflock.h>

#include "sx127x.h"

typedef enum
{
    FLOCK_SX127X_INTR_DIO0,
    FLOCK_SX127X_INTR_DIO1,
    FLOCK_SX127X_INTR_DIO2,
    FLOCK_SX127X_INTR_DIO3,
    FLOCK_SX127X_INTR_DIO4,
    FLOCK_SX127X_INTR_DIO5,
} flock_sx127x_intr_t;

typedef struct flock_config_s
{
    char name[12];
    uint8_t version[3];
    uint64_t radio_min_freq;
    uint64_t radio_max_freq;
    flock_band_t band;
} flock_config_t;

typedef struct flock_s
{
    sx127x_t sx127x;
    flock_config_t cfg;
    uint64_t frequency;
    flock_bird_info_t host_info;
    flock_posvel_t host_posvel;
    unsigned host_posvel_updated_ms;
} flock_t;

bool flock_init(flock_t *fl, const flock_config_t *cfg);

uint64_t flock_get_frequency(const flock_t *fl);
bool flock_set_frequency(flock_t *fl, uint64_t freq);

void flock_update(flock_t *fl);

void flock_sx127x_intr(flock_t *flock, flock_sx127x_intr_t intr);

void flock_serial_received(flock_t *flock, uint8_t c);

void flock_set_clkout_enabled(flock_t *fl, bool enabled);
