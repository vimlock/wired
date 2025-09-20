#include "../include/wired/wGraphics.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wPlatform.h"

struct _wTexture
{
	wNativeHandle handle;
	wPlatformOps *platform;
};

int wTextureFromImage(wTexture *tex, const wImage *img)
{
	wAssert(tex != NULL);
	wAssert(img != NULL);

	if (!tex || !img)
		return W_INVALID_ARGUMENT;

	wNativeHandle handle;

	if (!wPlatform->textureCreate)
		return W_NOT_SUPPORTED;

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

	// TODO

	wIVec2 tmp = { 0, 0 };
	return tmp;
}

wNativeHandle wTextureGetNativeHandle(wTexture *tex)
{
	wAssert(tex != NULL);

	return tex->handle;
}
