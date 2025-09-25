#include "../include/wired/wMemory.h"
#include "../include/wired/wError.h"

#include <stdlib.h>
#include <string.h>

void *wMemAlloc(size_t size)
{
	if (size == 0)
		size = 1;

	void *ret = malloc(size);
	if (!ret)
		wPanic("Out of memory");

	memset(ret, 0x0, size);

	return ret;
}

void *wMemRealloc(void *ptr, size_t size)
{
	if (size == 0)
		size = 1;

	void *ret = realloc(ptr, size);
	if (!ret)
		wPanic("Out of memory");

	return ret;
}

void wMemFree(void *ptr)
{
	if (!ptr)
		return;

	free(ptr);
}
