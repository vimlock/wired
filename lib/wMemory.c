#include "../include/wired/wMemory.h"

#include <stdlib.h>

void *wMemAlloc(size_t size)
{
	if (size == 0)
		size = 1;

	return malloc(size);
}

void *wMemRealloc(void *ptr, size_t size)
{
	if (size == 0)
		size = 1;

	return realloc(ptr, size);
}

void wMemFree(void *ptr)
{
	if (!ptr)
		return;

	free(ptr);
}
