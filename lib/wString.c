#include "../include/wired/wString.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wAssert.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

wString *wStringAlloc()
{
	wString *ret = wMemAlloc(sizeof(wString));
	ret->data = NULL;
	ret->size = 0;
	ret->capacity = 0;
	return ret;
}

void wStringFree(wString *str)
{
	wAssert(str != NULL);

	wMemFree(str->data);
	str->data = NULL;
	str->size = 0;
	str->capacity = 0;

	wMemFree(str);
}

wString *wStringCopy(const wString *src)
{
	wAssert(src != NULL);

	int err;

	wString *ret = wStringAlloc();
	wStringAssign(src, ret);
	return ret;
}

wString *wStringFromCString(const char *cstr)
{
	wAssert(cstr != NULL);

	int err;
	int len;

	wString *ret = wStringAlloc();

	len = strlen(cstr);
	wStringReserve(ret, len);

	ret->size = len;
	memcpy(ret->data, cstr, len + 1);

	return ret;
}

void wStringAssign(const wString *src, wString *dst)
{
	wStringReserve(dst, src->size);

	memcpy(dst->data, src->data, src->size);
	dst->size = src->size;
	dst->data[dst->size] = '\0';
}

wString *wStringFormat(const char *fmt, ...)
{
	wAssert(fmt != NULL);

	int size;
	int err;
	va_list ap;

	wString *ret = wStringAlloc();

	va_start(ap, fmt);
	size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	wStringReserve(ret, size);

	va_start(ap, fmt);
	vsnprintf(ret->data, size + 1, fmt, ap);
	va_end(ap);

	return ret;
}

void wStringClear(wString *str)
{
	wAssert(str != NULL);
	str->size = 0;
}

const char *wStringData(const wString *str)
{
	wAssert(str != NULL);
	return str->data;
}

size_t wStringSize(wString *str)
{
	wAssert(str != NULL);
	return str->size;
}

void wStringReserve(wString *str, size_t capacity)
{
	wAssert(str != NULL);

	if (str->capacity >= capacity)
		return;

	str->data = wMemRealloc(str->data, capacity + 1);
	str->capacity = capacity;
}

void wStringAppend(wString *str, size_t size, const char *data)
{
	wStringReserve(str, str->size + size);

	memcpy(str->data + str->size, data, size);
	str->size += size;
	str->data[str->size] = 0;
}

bool wStringEquals(const wString *a, const wString *b)
{
	wAssert(a != NULL);
	wAssert(b != NULL);

	if (a == b)
		return true;

	if (a->size != b->size)
		return false;

	return memcmp(a->data, b->data, a->size) == 0;
}
