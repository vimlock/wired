#include "../include/wired/wAssert.h"
#include "../include/wired/wLog.h"

#include <stdlib.h>

void _wAssert(bool cond, const char *msg, const char *file, int line)
{
	if (!cond) {
		wLogFatal("assertion (%s) at %s:%d failed", msg, file, line);
		abort();
	}
}
