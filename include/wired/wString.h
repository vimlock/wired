#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct _wString
{
	char *data;
	size_t size;
	size_t capacity;
} wString;

wString *wStringAlloc();
wString *wStringCopy(const wString *src);
void wStringAssign(wString *dst, const wString *src);
wString *wStringFromCString(const char *cstr);

void wStringFree(wString *str);

wString *wStringFormat(const char *fmt, ...);
void wStringAppend(wString *str, size_t size, const char *data);

bool wStringEquals(const wString *a, const wString *b);

void wStringClear(wString *str);

const char *wStringData(const wString *str);
size_t wStringSize(const wString *str);
void wStringReserve(wString *str, size_t capacity);
