#include <stdint.h>

#include "flock_cmd_handler.h"
#include "flock_crc.h"
#include "flock_log.h"
#include "flock_platform.h"

#define FLOCK_SERIAL_SYNC_BYTE_1 0xff
#define FLOCK_SERIAL_SYNC_BYTE_2 'F'
#define FLOCK_SERIAL_MAX_PACKET_SIZE 65
#define FLOCK_SERIAL_MAX_PAYLOAD_SIZE (FLOCK_SERIAL_MAX_PACKET_SIZE - 1) // 1 byte for cmd

typedef enum
{
    FLOCK_PROTOCOL_STATUS_NONE,
    FLOCK_PROTOCOL_STATUS_SYNC,
    FLOCK_PROTOCOL_STATUS_SIZE,
    FLOCK_PROTOCOL_STATUS_DATA,
    FLOCK_PROTOCOL_STATUS_CRC,
} flock_protocolStatus_t;

typedef struct flock_serial_state_s
{
    uint8_t buf[FLOCK_SERIAL_MAX_PACKET_SIZE];
    uint8_t bufPos;
    uint8_t crc;
    uint8_t packetSize;
    uint8_t status;
} flock_serial_state_t;

static flock_serial_state_t state;

static void flock_serial_reset_protocol_state(void)
{
    state.bufPos = 0;
    state.crc = 0;
    state.status = FLOCK_PROTOCOL_STATUS_NONE;
}

void flock_serial_received(flock_t *fl, uint8_t c)
{
    switch (state.status)
    {
    case FLOCK_PROTOCOL_STATUS_NONE:
        if (c == FLOCK_SERIAL_SYNC_BYTE_1)
        {
            state.status = FLOCK_PROTOCOL_STATUS_SYNC;
        }
        break;
    case FLOCK_PROTOCOL_STATUS_SYNC:
        if (c == FLOCK_SERIAL_SYNC_BYTE_2)
        {
            state.status = FLOCK_PROTOCOL_STATUS_SIZE;
            break;
        }
        flock_serial_reset_protocol_state();
        break;
    case FLOCK_PROTOCOL_STATUS_SIZE:
        if (c <= FLOCK_SERIAL_MAX_PACKET_SIZE)
        {
            state.crc = flock_crc_dvb_s2(state.crc, c);
            state.packetSize = c;
            state.status = FLOCK_PROTOCOL_STATUS_DATA;
            break;
        }
        flock_serial_reset_protocol_state();
        break;
    case FLOCK_PROTOCOL_STATUS_DATA:
        state.crc = flock_crc_dvb_s2(state.crc, c);
        state.buf[state.bufPos++] = c;
        if (state.bufPos == state.packetSize)
        {
            state.status = FLOCK_PROTOCOL_STATUS_CRC;
        }
        break;
    case FLOCK_PROTOCOL_STATUS_CRC:
        if (c != state.crc)
        {
            FLOG_W("Invalid CRC, got %02x != %02x", c, state.crc);
            flock_serial_reset_protocol_state();
            break;
        }
        flock_handle_command(fl, state.buf, state.bufPos);
        flock_serial_reset_protocol_state();
        break;
    }
}

int flock_serial_write_cmd(uint8_t cmd, const void *payload, size_t size)
{
    if (size > FLOCK_SERIAL_MAX_PAYLOAD_SIZE)
    {
        return -2;
    }

    flock_serial_write(FLOCK_SERIAL_SYNC_BYTE_1);
    flock_serial_write(FLOCK_SERIAL_SYNC_BYTE_2);
    int packet_size = 1;
    uint8_t crc;
    if (payload)
    {
        packet_size += size;
    }
    else
    {
        size = 0;
    }

    crc = flock_crc_dvb_s2(0, packet_size);
    crc = flock_crc_dvb_s2(crc, cmd);

    flock_serial_write(packet_size);
    flock_serial_write(cmd);
    const uint8_t *ptr = payload;
    const uint8_t *end = ptr + size;
    for (; ptr < end; ptr++)
    {
        flock_serial_write(*ptr);
        crc = flock_crc_dvb_s2(crc, *ptr);
    }
    flock_serial_write(crc);
    return size;
}
