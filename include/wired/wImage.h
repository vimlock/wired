#pragma once

#include "wMath.h"

typedef struct _wImage wImage;
typedef struct _wTexture wTexture;
typedef struct _wString wString;
typedef struct _wBuffer wBuffer;

enum wImageFormat
{
    W_IMAGE_RGBA8,
    W_IMAGE_RGB8,
    W_IMAGE_GRAYSCALE8,
};

wImage *wImageAlloc();
int wImageInit(wImage *img, int width, int height, int format);
void wImageFree(wImage *img);
int wImageLoad(wImage *img, const wString *path);
int wImageSave(wImage *img, const wString *path);
void wImageFill(wImage *img, int r, int g, int b, int a);
wImage *wImageCopy(const wImage *src);
wImage *wImageCrop(const wImage *src, wRectI rect);
wTexture *wImageGetTexture(wImage *img);
char *wImageData(wImage *img);
int wImageFormat(const wImage *img);
int wImageWidth(const wImage *img);
int wImageHeight(const wImage *img);
int wImagePixelSize(int format);
wIVec2 wImageSize(const wImage *img);