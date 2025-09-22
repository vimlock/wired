#pragma once

#include "wMath.h"

#include <stdint.h>

typedef struct _wTexture wTexture;
typedef struct _wImage wImage;
typedef uintptr_t wNativeHandle;

enum wTextureFilter
{
	W_TEXTURE_NEAREST,
	W_TEXTURE_BILINEAR,
};

enum wTextureWrap
{
	W_TEXTURE_CLAMP,
	W_TEXTURE_REPEAT,
};

wTexture *wTextureAlloc();
void wTextureFree(wTexture *tex);
int wTextureLoadFromImage(wTexture *tex, const wImage *img);
int wTextureGenMipMaps(wTexture *tex);
int wTextureSetFilter(wTexture *tex, int mode);
int wTextureSetWrap(wTexture *tex, int mode);
wNativeHandle wTextureGetNativeHandle(wTexture *tex);
wIVec2 wTextureGetSize(wTexture *tex);