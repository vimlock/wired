#include "../include/wired/wBuffer.h"
#include "../include/wired/wAssert.h"
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

void wBufferResize(wBuffer *buf, size_t newSize)
{
	wAssert(buf != NULL);

	wBufferReserve(buf, newSize);
	buf->size = newSize;
}

void wBufferReserve(wBuffer *buf, size_t capacity)
{
	wAssert(buf != NULL);

	if (buf->capacity >= capacity)
		return;

	buf->data = wMemRealloc(buf->data, capacity);
	buf->capacity = capacity;
}

void wBufferAppend(wBuffer *buf, size_t size, const void *data)
{
	wAssert(buf != NULL);
	wAssert(data != NULL);

	int err;

	wBufferReserve(buf, buf->size + size);
	memcpy(((char*)buf->data) + buf->size, data, size);
	buf->size += size;
}
