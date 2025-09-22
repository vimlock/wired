#include "../include/wired/wBuffer.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"

#include <string.h>

void wBufferInit(wBuffer *buf)
{
	wAssert(buf != NULL);

	buf->size = 0;
	buf->data = 0;
	buf->capacity = 0;
}

void wBufferFree(wBuffer *buf)
{
	wAssert(buf != NULL);

	wMemFree(buf->data);
	buf->data = NULL;
	buf->size = 0;
	buf->capacity = 0;
}

void wBufferClear(wBuffer *buf)
{
	wAssert(buf != NULL);
	buf->size = 0;
}

int wBufferResize(wBuffer *buf, size_t newSize)
{
	wAssert(buf != NULL);

	int err;
	err = wBufferReserve(buf, newSize);
	if (err)
		return err;

	buf->size = newSize;

	return W_SUCCESS;
}

int wBufferReserve(wBuffer *buf, size_t capacity)
{
	wAssert(buf != NULL);

	if (buf->capacity >= capacity)
		return W_SUCCESS;

	char *newmem = wMemRealloc(buf->data, capacity);
	if (!newmem)
		return W_OUT_OF_MEMORY;

	buf->data = newmem;
	buf->capacity = capacity;

	return W_SUCCESS;
}

int wBufferAppend(wBuffer *buf, size_t size, const void *data)
{
	wAssert(buf != NULL);
	wAssert(data != NULL);

	int err;

	err = wBufferReserve(buf, buf->size + size);
	if (err)
		return err;

	memcpy(((char*)buf->data) + buf->size, data, size);
	buf->size += size;

	return W_SUCCESS;
}
