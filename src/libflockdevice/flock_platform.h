#pragma once

#include <stddef.h>
#include <stdint.h>

#include <libflock.h>

unsigned flock_millis(void);

void flock_device_addr_get(flock_addr_t *addr);

int flock_serial_write(uint8_t c);

void flock_sx127x_set_rst(unsigned val);
void flock_sx127x_spi_transmit(uint16_t cmd, uint32_t addr,
                               const void *tx, size_t tx_size,
                               void *rx, size_t rx_size);
