#include <stdint.h>
#include <string.h>

#include <libflock.h>

#include "flock.h"
#include "flock_cmd_handler.h"
#include "flock_log.h"
#include "flock_platform.h"
#include "flock_serial_private.h"

#define FLOCK_DEFAULT_HOST_POSVEL_INTERVAL_MS 500

int flock_handle_command(flock_t *fl, const void *buf, size_t size)
{
    if (size < 1)
    {
        return -1;
    }
    const uint8_t *ptr = buf;
    flock_cmd_t cmd = *ptr;
    ptr++;
    switch (cmd)
    {
    case FLOCK_CMD_DEVICE_INFO:
    {
        flock_dev_info_t info = {
            .flock_version = LIBFLOCK_FLOCK_VERSION,
            .host_posvel_interval_ms = FLOCK_DEFAULT_HOST_POSVEL_INTERVAL_MS,
            .device_radio_type = FLOCK_RADIO_TYPE_LORA_SUB_GHZ,
            .device_radio_min_freq = fl->cfg.radio_min_freq,
            .device_radio_max_freq = fl->cfg.radio_max_freq,
            .device_radio_default_freq = flock_band_get_default_freq(fl->cfg.band),
        };
        memcpy(info.device_name, fl->cfg.name, sizeof(info.device_name));
        memcpy(info.device_version, fl->cfg.version, sizeof(info.device_version));
        flock_device_addr_get(&info.device_addr);
        flock_serial_write_cmd(FLOCK_CMD_DEVICE_INFO, &info, sizeof(info));
        break;
    }
    case FLOCK_CMD_GET_FREQ:
    {
        uint64_t freq = flock_get_frequency(fl);
        flock_serial_write_cmd(FLOCK_CMD_GET_FREQ, &freq, sizeof(freq));
        break;
    }
    case FLOCK_CMD_SET_FREQ:
    {
        if (size >= sizeof(uint64_t))
        {
            uint64_t freq;
            memcpy(&freq, buf, sizeof(freq));
            if (!flock_set_frequency(fl, freq))
            {
                // No frequency change performed, return the previous one
                freq = flock_get_frequency(fl);
            }
            flock_serial_write_cmd(FLOCK_CMD_SET_FREQ, &freq, sizeof(freq));
        }
        break;
    }
    case FLOCK_CMD_GET_HOST_BIRD_INFO:
    {
        flock_serial_write_cmd(FLOCK_CMD_GET_HOST_BIRD_INFO, &fl->host_info, sizeof(fl->host_info));
        break;
    }
    case FLOCK_CMD_SET_HOST_BIRD_INFO:
    {
        if (size >= sizeof(fl->host_info))
        {
            memcpy(&fl->host_info, buf, sizeof(fl->host_info));
        }
        flock_serial_write_cmd(FLOCK_CMD_SET_HOST_BIRD_INFO, &fl->host_info, sizeof(fl->host_info));
        break;
    }
    case FLOCK_CMD_SET_HOST_POSVEL:
    {
        if (size >= sizeof(fl->host_posvel))
        {
            memcpy(&fl->host_posvel, buf, sizeof(fl->host_posvel));
            fl->host_posvel_updated_ms = flock_millis();
        }
        break;
    }
    case FLOCK_CMD_BROADCAST_DATA:
    {
        // TODO
        break;
    }
    case FLOCK_CMD_SEND_DATA:
    {
        // TODO
        break;
    }
    // Commands that shouldn't be sent by the host
    case FLOCK_CMD_REMOTE_BIRD_INFO:
    case FLOCK_CMD_REMOTE_BIRD_POSVEL:
    case FLOCK_CMD_RECV_BROADCAST_DATA:
    case FLOCK_CMD_RECV_SENT_DATA:
        break;
    }
}
