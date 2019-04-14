#pragma once

#include <stddef.h>
#include <stdint.h>

int flock_serial_write_cmd(uint8_t cmd, const void *payload, size_t size);
