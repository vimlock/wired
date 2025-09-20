#include "../include/wired/wArgs.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"

#include <stddef.h>

static const char **wArgv = NULL;
static int wArgc = 0;

int wArgsParse(int argc, const char **argv)
{
	wArgv = argv + 1;
	wArgc = argc - 1;

	return W_SUCCESS;
}

int wArgsGetNumPositional()
{
	return wArgc;
}

const char *wArgsGetPositional(int index)
{
	wAssert(index >= 0 && index < wArgc);

	if (index < 0 || index >= wArgc)
		return NULL;

	return wArgv[index];
}
