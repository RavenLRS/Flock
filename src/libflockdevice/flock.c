#include <string.h>

#include "flock.h"

bool flock_init(flock_t *fl, const flock_config_t *cfg)
{
    memcpy(&fl->cfg, cfg, sizeof(fl->cfg));
    memset(&fl->host_info, 0, sizeof(fl->host_info));
    memset(&fl->host_posvel, 0, sizeof(fl->host_posvel));
    fl->host_posvel_updated_ms = 0;

    fl->sx127x.output_type = SX127X_OUTPUT_PA_BOOST;
    if (!sx127x_init(&fl->sx127x))
    {
        return false;
    }
    // Clock divider can only be changed in FSK, which is
    // the mode the SX127X starts in. Change the clock divider
    // to 4 before we switch to LoRa mode.
    //sx127x_set_op_mode(&fl->sx127x, SX127X_OP_MODE_FSK);
    sx127x_set_clkout_div(&fl->sx127x, SX127X_CLKOUT_DIV_4);

    // Initialize PRNG
    flock_prng_seed(&fl->prng, sx127x_random(&fl->sx127x));

    sx127x_set_op_mode(&fl->sx127x, SX127X_OP_MODE_LORA);
    sx127x_idle(&fl->sx127x);

    flock_set_frequency(fl, flock_band_get_default_freq(cfg->band));
    return true;
}

void flock_update(flock_t *fl)
{
}

uint64_t flock_get_frequency(const flock_t *fl)
{
    return fl->frequency;
}

bool flock_set_frequency(flock_t *fl, uint64_t freq)
{
    if (freq >= fl->cfg.radio_min_freq && freq <= fl->cfg.radio_max_freq)
    {
        fl->frequency = freq;
        // TODO: image calibration
        sx127x_set_frequency(&fl->sx127x, freq, 0);
        return true;
    }
    return false;
}

void flock_set_clkout_enabled(flock_t *fl, bool enabled)
{
    sx127x_set_clkout_enabled(&fl->sx127x, enabled);
}
