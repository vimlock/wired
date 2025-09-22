#pragma once

enum wError
{
	W_SUCCESS = 0,
	W_INVALID_OPERATION,
	W_INVALID_ARGUMENT,
	W_IO_ERROR,
	W_NOT_IMPLEMENTED,
	W_NOT_SUPPORTED,
	W_PARSE_ERROR,
};

const char *wErrorStr(int error);
void wPanic(const char *msg);
