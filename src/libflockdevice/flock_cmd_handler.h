#pragma once

#include <stddef.h>

typedef struct flock_s flock_t;

int flock_handle_command(flock_t *fl, const void *buf, size_t size);
