#include "../include/wired/wImage.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"

#include <string.h>

int wImagePNGLoad(wImage *img, const wString *path);

struct _wImage
{
	void *data;
	int width;
	int height;
	int format;
};

int wImagePixelSize(int format)
{
	switch (format) {
	case W_IMAGE_RGBA8:      return 4;
	case W_IMAGE_RGB8:       return 3;
	case W_IMAGE_GRAYSCALE8: return 1;
	default:
		wAssert(0 && "Invalid image format");
		return 0;
	}
}

wImage *wImageAlloc()
{
	wImage *ret = wMemAlloc(sizeof(wImage));
	memset(ret, 0x0, sizeof(wImage));

	ret->width = 0;
	ret->height = 0;
	ret->format = W_IMAGE_RGBA8;
	ret->data = NULL;

	return ret;
}

int wImageInit(wImage *img, int width, int height, int format)
{
	wAssert(img != NULL);

	if (!img)
		return W_INVALID_ARGUMENT;

	if (width <= 0 || height <= 0)
		return W_INVALID_ARGUMENT;

	int bpp = wImagePixelSize(format);

	img->data = wMemRealloc(img->data, width * height * bpp);

	img->width = width;
	img->height = height;
	img->format = format;

	return W_SUCCESS;
}

void wImageFree(wImage *img)
{
	if (!img)
		return;

	if (img->data)
		wMemFree(img->data);

	wMemFree(img);
}

int wImageLoad(wImage *img, const wString *path)
{
	wAssert(img != NULL);
	wAssert(path != NULL);

	int err;

	if (!img || !path)
		return W_INVALID_ARGUMENT;

	err = wImagePNGLoad(img, path);
	if (!err) {
		wLogDebug("Loaded image %dx%d", img->width, img->height);
	}

	return err;
}

int wImageSave(wImage *img, const wString *path)
{
	wAssert(img != NULL);
	wAssert(path != NULL);

	if (!img || !path)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

void wImageFill(wImage *img, int r, int g, int b, int a)
{
	wAssert(img != NULL);
}

wImage *wImageCopy(const wImage *src)
{
	wAssert(src != NULL);

	int imgDataSize = wImageWidth(src) * wImageHeight(src) * wImagePixelSize(src->format);

	wImage *ret = wImageAlloc();
	wImageInit(ret, src->width, src->height, src->format);
	memcpy(ret->data, src->data, imgDataSize);
	return ret;
}

wImage *wImageCrop(const wImage *src, wRectI rect)
{
	wAssert(src != NULL);

	wRectI selfRect = {0, 0, src->width, src->height};
	rect = wRectIIntersect(rect, selfRect);

	wImage *ret = wImageAlloc();
	wImageInit(ret, rect.w, rect.h, src->format);

	int bpp = wImagePixelSize(src->format);
	int rowSize = rect.w * bpp;

	/* Copy row by row*/
	for (int y = 0; y < rect.h; ++y) {
		int srcY = rect.y + y;

		memcpy((char*)ret->data + y * rowSize,
		       (char*)src->data + (srcY * src->width + rect.x) * bpp,
		       rowSize);
	}

	return ret;
}

wTexture *wImageGetTexture(wImage *img)
{
	return NULL;
}

char *wImageData(wImage *img)
{
	wAssert(img != NULL);
	return img->data;
}

int wImageFormat(const wImage *img)
{
	wAssert(img != NULL);
	return img->format;
}

int wImageWidth(const wImage *img)
{
	wAssert(img != NULL);
	return img->width;
}

int wImageHeight(const wImage *img)
{
	wAssert(img != NULL);
	return img->height;
}

wIVec2 wImageSize(const wImage *img)
{
	wAssert(img != NULL);
	wIVec2 ret = {img->width, img->height};
	return ret;
}