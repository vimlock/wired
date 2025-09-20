#include "../include/wired/wString.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

void wStringInit(wString *str)
{
	wAssert(str != NULL);

	str->data = NULL;
	str->size = 0;
	str->capacity = 0;
}

void wStringFree(wString *str)
{
	wAssert(str != NULL);

	wMemFree(str->data);
	str->data = NULL;
	str->size = 0;
	str->capacity = 0;
}

int wStringCopy(const wString *src, wString *dst)
{
	wAssert(src != NULL);
	wAssert(dst != NULL);

	int err;

	err = wStringReserve(dst, src->size);
	if (err)
		return err;

	memcpy(dst->data, src->data, src->size);
	dst->size = src->size;
	dst->data[dst->size] = '\0';

	return W_SUCCESS;
}

int wStringFromCString(wString *str, const char *cstr)
{
	wAssert(str != NULL);
	wAssert(cstr != NULL);

	int err;
	int len;

	len = strlen(cstr);

	err = wStringReserve(str, len);
	if (err)
		return err;

	str->size = len;
	memcpy(str->data, cstr, len + 1);

	return W_SUCCESS;
}

int wStringFormat(wString *str, const char *fmt, ...)
{
	wAssert(str != NULL);
	wAssert(fmt != NULL);

	int size;
	int err;
	va_list ap;

	va_start(ap, fmt);
	size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (size < 0)
		return W_INVALID_ARGUMENT;

	err = wStringReserve(str, size);
	if (err)
		return err;

	va_start(ap, fmt);
	vsnprintf(str->data, size + 1, fmt, ap);
	va_end(ap);

	return W_NOT_IMPLEMENTED;
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

int wStringReserve(wString *str, size_t capacity)
{
	wAssert(str != NULL);

	if (str->capacity >= capacity)
		return W_SUCCESS;

	char *newmem = wMemRealloc(str->data, capacity + 1);
	if (!newmem)
		return W_OUT_OF_MEMORY;

	str->data = newmem;
	str->capacity = capacity;

	return W_SUCCESS;
}

int wStringAppend(wString *str, size_t size, const char *data)
{
	int err;

	err = wStringReserve(str, str->size + size);
	if (err)
		return err;

	memcpy(str->data + str->size, data, size);
	str->size += size;
	str->data[str->size] = 0;

	return W_SUCCESS;
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
