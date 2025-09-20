#pragma once

#include <stddef.h>

void *wMemAlloc(size_t size);
void *wMemRealloc(void *ptr, size_t size);
void wMemFree(void *ptr);
