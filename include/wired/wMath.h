#pragma once

#include <stdbool.h>

typedef struct _wMat4
{
	float m[4][4];
} wMat4;

typedef struct _wVec2
{
	float x;
	float y;
} wVec2;

typedef struct _wIVec2
{
	int x;
	int y;
} wIVec2;

typedef struct _wVec3
{
	float x;
	float y;
	float z;
} wVec3;

typedef struct _wVec4
{
	float x;
	float y;
	float z;
	float w;
} wVec4;

typedef struct _wRect
{
	float x;
	float y;
	float w;
	float h;
} wRect;

typedef struct _wRectI
{
	int x;
	int y;
	int w;
	int h;
} wRectI;

typedef struct _wColor
{
	float r;
	float g;
	float b;
	float a;
} wColor;

wVec2 wVec2Normalized(wVec2 vec);
wVec3 wVec3Normalized(wVec3 vec);

wRect wRectNormalize(wRect rect);
wRect wRectShrink(wRect rect, float amount);
wRect wRectGrow(wRect rect, float amount);
bool wRectContains(wRect rect, float x, float y);

wRectI wRectIIntersect(wRectI a, wRectI b);
wRectI wRectIShrink(wRectI rect, int amount);
wRectI wRectIGrow(wRectI rect, int amount);

void wMat4Zero(wMat4 *m);
void wMat4Ortho(wMat4 *m, float left, float right, float top, float bottom, float near, float far);
void wMat4Identity(wMat4 *m);
void wMat4Translate(wMat4 *m, float x, float y, float z);
void wMat4Transpose(wMat4 *m);
void wMat4Multiply(const wMat4 *a, const wMat4 *b, wMat4 *ret);
wVec4 wMat4MultiplyPoint(const wMat4 *m, wVec4 v);
