#include "../include/wired/wError.h"
#include "../include/wired/wLog.h"

#include <stdlib.h>

const char *wErrorStr(int error)
{
	switch (error) {
		case W_SUCCESS: return "SUCCESS";
		case W_INVALID_OPERATION: return "INVALID_OPERATION";
		case W_INVALID_ARGUMENT: return "INVALID_ARGUMENT";
		case W_NO_SUCH_FILE:     return "NO_SUCH_FILE";
		case W_IO_ERROR:         return "IO_ERROR";
		case W_NOT_IMPLEMENTED:  return "NOT_IMPLEMENTED";
		case W_NOT_SUPPORTED:    return "NOT_SUPPORTED";
		case W_PARSE_ERROR:      return "PARSE_ERROR";
	}

	return "UNKNOWN";
}

void wPanic(const char *msg)
{
	wLogFatal("PANIC: %s", msg);
	abort();
}
