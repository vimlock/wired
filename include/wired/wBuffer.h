#pragma once

#include <stddef.h>

typedef struct _wBuffer
{
	size_t size;
	void *data;
	size_t capacity;
} wBuffer;

void wBufferInit(wBuffer *buf);
void wBufferFree(wBuffer *buf);

void *wBufferData(wBuffer *buf);
size_t wBufferSize(const wBuffer *buf);

void wBufferClear(wBuffer *buf);
void wBufferSwap(wBuffer *a, wBuffer *b);

int wBufferResize(wBuffer *buf, size_t newSize);
int wBufferReserve(wBuffer *buf, size_t newCapacity);

int wBufferAppend(wBuffer *buf, size_t size, const void *data);
