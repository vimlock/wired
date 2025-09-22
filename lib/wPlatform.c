#include "../include/wired/wPlatform.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"

#include <string.h>

wPlatformOps *wPlatform;

static bool wShouldQuit = false;

extern int wSdlInit(wPlatformOps *p);
extern int wGlInit(wPlatformOps *p);

int wPlatformInit()
{
	wAssert(wPlatform == NULL);

	int err;

	wPlatform = wMemAlloc(sizeof(wPlatformOps));
	memset(wPlatform, 0x0, sizeof(wPlatformOps));

	err = wSdlInit(wPlatform);
	if (err) {
		wMemFree(wPlatform);
		wPlatform = NULL;
		return err;
	}

	return W_SUCCESS;
}

void wPlatformFree()
{
	if (!wPlatform)
		return;

	wMemFree(wPlatform);
	wPlatform = NULL;
}

bool wApplicationShouldQuit()
{
	return wShouldQuit;
}

void wApplicationQuit()
{
	if (wShouldQuit)
		return;

	wShouldQuit = true;
	wLogInfo("Quit requested");
}
