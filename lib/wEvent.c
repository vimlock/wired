#include "../include/wired/wEvent.h"
#include "../include/wired/wPlatform.h"
#include "../include/wired/wMemory.h"

struct _wEventQueue
{
	int head;
	int tail;
	int size;
	int capacity;
	wEvent *data;
};

static struct _wEventQueue queue;

void wEventInit()
{
	queue.capacity = 1024;
	queue.data = wMemAlloc(queue.capacity * sizeof(wEvent));
	queue.head = 0;
	queue.tail = -1;
}

void wEventFree()
{
	wMemFree(queue.data);
	queue.data = NULL;
}

void wEventPump()
{
	wPlatformOps *platform = wPlatform;
	platform->windowPollEvents();
}

bool wEventCanPush()
{
	return queue.size < queue.capacity;
}

bool wEventPush(const wEvent *evt)
{
	if (!wEventCanPush())
		return false;

	queue.tail = (queue.tail + 1) % queue.capacity;
	queue.data[queue.tail] = *evt;
	queue.size++;
	return true;
}

bool wEventPoll(wEvent *ret)
{
	if (queue.size == 0)
		return false;

	*ret = queue.data[queue.head];
	queue.head = (queue.head + 1) % queue.capacity;
	queue.size--;

	return true;
}
