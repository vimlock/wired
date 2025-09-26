#include "../include/wired/wTexture.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wPlatform.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wImage.h"
#include "../include/wired/wLog.h"

#include <string.h>

static wClass wTextureClass =
{
	.name = "Texture",
	.base = NULL,
	.version = 1
};

struct _wTexture
{
	const wClass *class;
	wNativeHandle handle;
	wPlatformOps *platform;

	int width;
	int height;
	int format;
};

static void release(wTexture *tex)
{
	if (!tex->handle)
		return;

	tex->platform->textureDestroy(tex->handle);
	tex->handle = 0;
}

wTexture *wTextureAlloc()
{
	wTexture *tex = wMemAlloc(sizeof(wTexture));

	tex->class = &wTextureClass;
	tex->handle = 0;
	tex->platform = NULL;

	return tex;
}

void wTextureFree(wTexture *tex)
{
	if (!tex)
		return;

	release(tex);
	wMemFree(tex);
}

int wTextureLoadFromImage(wTexture *tex, const wImage *img)
{
	wAssert(tex != NULL);
	wAssert(img != NULL);

	if (!tex || !img)
		return W_INVALID_ARGUMENT;

	if (wImageWidth(img) < 1 || wImageHeight(img) < 1)
		return W_INVALID_ARGUMENT;

	wNativeHandle handle;

	if (!wPlatform->textureCreate || !wPlatform->textureDestroy)
		return W_NOT_SUPPORTED;

	tex->platform = wPlatform;
	tex->width = wImageWidth(img);
	tex->height = wImageHeight(img);
	tex->format = wImageFormat(img);
	
	wLogDebug("Creating texture %dx%d (fmt=%d)", tex->width, tex->height, tex->format);
	tex->handle = wPlatform->textureCreate(
		tex->width,
		tex->height,
		tex->format
	);

	wPlatform->textureData(
		tex->handle,
		0,
		0,
		tex->width,
		tex->height,
		W_IMAGE_RGBA8,
		wImageData((wImage*)img)
	);

	wLogDebug("Uploaded to GPU as texture %d", (int)tex->handle);

	return W_SUCCESS;
}

int wTextureGenMipMaps(wTexture *tex)
{
	wAssert(tex != NULL);

	if (!tex)
		return W_INVALID_ARGUMENT;

	if (!wPlatform->textureGenMipMaps)
		return W_NOT_SUPPORTED;

	return wPlatform->textureGenMipMaps(tex->handle);
}

int wTextureSetFilter(wTexture *tex, int mode)
{
	wAssert(tex != NULL);

	if (!tex)
		return W_INVALID_ARGUMENT;

	if (!wPlatform->textureSetFilter)
		return W_NOT_SUPPORTED;

	return wPlatform->textureSetFilter(tex->handle, mode);
}

int wTextureSetWrap(wTexture *tex, int mode)
{
	wAssert(tex != NULL);

	if (!tex)
		return W_INVALID_ARGUMENT;

	if (!wPlatform->textureSetWrap)
		return W_NOT_SUPPORTED;

	return wPlatform->textureSetWrap(tex->handle, mode);
}

wIVec2 wTextureGetSize(wTexture *tex)
{
	wAssert(tex != NULL);

	wIVec2 tmp = { tex->width, tex->height };
	return tmp;
}

int wTextureGetWidth(const wTexture *tex)
{
	wAssert(tex != NULL);
	return tex->width;
}

int wTextureGetHeight(const wTexture *tex)
{
	wAssert(tex != NULL);
	return tex->height;
}

wNativeHandle wTextureGetNativeHandle(wTexture *tex)
{
	wAssert(tex != NULL);

	return tex->handle;
}
