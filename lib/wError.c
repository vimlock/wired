#include "../include/wired/wError.h"

const char *wErrorStr(int error)
{
	switch (error) {
		case W_SUCCESS: return "SUCCESS";
		case W_INVALID_OPERATION: return "INVALID_OPERATION";
		case W_INVALID_ARGUMENT: return "INVALID_ARGUMENT";
		case W_IO_ERROR:         return "IO_ERROR";
		case W_OUT_OF_MEMORY:    return "OUT_OF_MEMORY";
		case W_NOT_IMPLEMENTED:  return "NOT_IMPLEMENTED";
		case W_NOT_SUPPORTED:    return "NOT_SUPPORTED";
		case W_PARSE_ERROR:      return "PARSE_ERROR";
	}

	return "UNKNOWN";
}
