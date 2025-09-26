#pragma once

#include "wMath.h"

#include <stdint.h>
#include <stddef.h>

typedef struct _wVertex wVertex;
typedef struct _wString wString;
typedef struct _wImage wImage;

struct _wVertex
{
	float posX;
	float posY;
	float posZ;
	float texX;
	float texY;
};

typedef uint16_t wIndex;
typedef uintptr_t wNativeHandle;

typedef struct _wPlatformOps
{
	int (*windowCreate)(int w, int h);
	void (*windowDestroy)();

	int (*windowSetIcon)(const wImage *img);
	int (*windowSetTitle)(const wString *title);
	int (*windowSetFullscreen)();
	int (*windowMaximize)();
	int (*windowMinimize)();
	int (*windowGetGeometry)(wRectI *ret);
	int (*windowSwapBuffers)();
	int (*windowPollEvents)();

	int (*screenGetCount)();

	void * (*getProcAddress)(const char *name);

	wNativeHandle (*shaderCreate)();
	void (*shaderDestroy)(wNativeHandle shader);
	int (*shaderCompile)(wNativeHandle shader, const wString *vert, const wString *frag);
	int (*shaderSetValue)(wNativeHandle shader, int location, int type, const void *data);
	int (*shaderBind)(wNativeHandle shader);
	int (*shaderGetUniformLocation)(wNativeHandle shader, const char *name);

	wNativeHandle (*textureCreate)(int w, int h, int fmt);
	void (*textureDestroy)(wNativeHandle tex);
	int (*textureGenMipMaps)(wNativeHandle tex);
	int (*textureSetFilter)(wNativeHandle tex, int mode);
	int (*textureSetWrap)(wNativeHandle tex, int mode);
	int (*textureBind)(wNativeHandle tex, int index);
	int (*textureData)(wNativeHandle tex, int x, int y, int w, int h, int fmt, const void *data);

	wNativeHandle (*bufferCreate)(size_t size, const void *data);
	void (*bufferDestroy)(wNativeHandle buf);
	int (*bufferData)(wNativeHandle buf, size_t size, const void *data);

	int (*draw)(int numElements, wNativeHandle vbo, wNativeHandle ibo);
	int (*clear)(float r, float g, float b, float a);

	int (*setScissor)(int x, int y, int w, int h);
	int (*setViewport)(int x, int y, int w, int h);
	int (*setBlend)(int enabled);
} wPlatformOps;

int wPlatformInit();
void wPlatformFree();

bool wApplicationShouldQuit();
void wApplicationQuit();

extern wPlatformOps *wPlatform;
