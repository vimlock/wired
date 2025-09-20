#include "../include/wired/wGraphics.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wPlatform.h"

int wWindowOpen(int width, int height)
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowCreate)
		return W_NOT_SUPPORTED;

	return wPlatform->windowCreate(width, height);
}

void wWindowClose()
{
	wAssert(wPlatform != NULL);

	if (wPlatform->windowDestroy)
		wPlatform->windowDestroy();
}

int wWindowSetIcon(const wImage *img)
{
	wAssert(wPlatform != NULL);
	wAssert(img != NULL);

	if (!img)
		return W_INVALID_ARGUMENT;

	if (!wPlatform->windowSetIcon)
		return W_NOT_SUPPORTED;

	return wPlatform->windowSetIcon(img);
}

int wWindowSetTitle(const wString *title)
{
	wAssert(wPlatform != NULL);
	wAssert(title != NULL);

	if (!title)
		return W_INVALID_ARGUMENT;

	if (!wPlatform->windowSetTitle)
		return W_NOT_SUPPORTED;

	return wPlatform->windowSetTitle(title);
}

int wWindowSetFullscreen()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowSetFullscreen)
		return W_NOT_SUPPORTED;

	return wPlatform->windowSetFullscreen();
}

int wWindowMaximize()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowMaximize)
		return W_NOT_SUPPORTED;

	return wPlatform->windowMaximize();
}

int wWindowMinimize()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowMinimize)
		return W_NOT_SUPPORTED;

	return wPlatform->windowMinimize();
}

int wWindowGetWidth()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowGetGeometry)
		return 0;

	wRectI geom;
	int err;

	err = wPlatform->windowGetGeometry(&geom);
	if (err)
		return 0;

	return geom.w;
}

int wWindowGetHeight()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowGetGeometry)
		return 0;

	wRectI geom;
	int err;

	err = wPlatform->windowGetGeometry(&geom);
	if (err)
		return 0;

	return geom.h;
}

int wWindowSwapBuffers()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowSwapBuffers)
		return W_NOT_SUPPORTED;

	return wPlatform->windowSwapBuffers();
}

int wWindowPollEvents()
{
	wAssert(wPlatform != NULL);

	if (!wPlatform->windowPollEvents)
		return W_NOT_SUPPORTED;

	return wPlatform->windowPollEvents();
}
