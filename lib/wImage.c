#include "../include/wired/wGraphics.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"

struct _wImage
{
};

int wImageLoad(wImage *img, const wString *name, const wBuffer *data)
{
	wAssert(img != NULL);
	wAssert(name != NULL);
	wAssert(data != NULL);

	if (!img || !name || !data)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wImageSave(wImage *img, const wString *name, wBuffer *data)
{
	wAssert(img != NULL);
	wAssert(name != NULL);
	wAssert(data != NULL);

	if (!img || !name || !data)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

void wImageFill(wImage *img, int r, int g, int b, int a)
{
	wAssert(img != NULL);
}

int wImageCopy(const wImage *src, wImage *dst)
{
	wAssert(src != NULL);
	wAssert(dst != NULL);

	if (!src || !dst)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wImageCrop(const wImage *src, wImage *dst, wRectI rect)
{
	wAssert(src != NULL);
	wAssert(dst != NULL);

	if (!src || !dst)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

wTexture *wImageGetTexture(wImage *img)
{
	return NULL;
}
