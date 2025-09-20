#pragma once

#include <stdbool.h>

#define wAssert(cond) (_wAssert(cond, #cond, __FILE__, __LINE__))

void _wAssert(bool cond, const char *msg, const char *file, int line);
