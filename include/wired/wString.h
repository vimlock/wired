#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct _wString
{
	char *data;
	size_t size;
	size_t capacity;
} wString;

void wStringInit(wString *str);
void wStringFree(wString *str);

int wStringCopy(const wString *src, wString *dst);

int wStringFromCString(wString *str, const char *cstr);

int wStringFormat(wString *str, const char *fmt, ...);

int wStringAppend(wString *str, size_t size, const char *data);

bool wStringEquals(const wString *a, const wString *b);

void wStringClear(wString *str);

const char *wStringData(const wString *str);
size_t wStringSize(wString *str);
int wStringReserve(wString *str, size_t capacity);
