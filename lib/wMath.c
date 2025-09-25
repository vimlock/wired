#include "../include/wired/wMath.h"

#include <math.h>
#include <string.h>

wVec2 wVec2Normalized(wVec2 vec)
{
	float l = sqrt(vec.x * vec.x + vec.y * vec.y);
	vec.x /= l;
	vec.y /= l;
	return vec;
}

wVec3 wVec3Normalized(wVec3 vec)
{
	float l = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= l;
	vec.y /= l;
	vec.z /= l;
	return vec;
}

wRect wRectNormalize(wRect rect)
{
	if (rect.w < 0) {
		rect.x -= rect.w;
		rect.w = -rect.w;
	}

	if (rect.h < 0) {
		rect.y -= rect.h;
		rect.y = -rect.h;
	}

	return rect;
}

wRect wRectShrink(wRect rect, float amount)
{
	float sx = amount;
	float sy = amount;

	if (sx > rect.w)
		sx = rect.w;
	if (sy > rect.h)
		sy = rect.h;

	rect.x += sx;
	rect.y += sy;
	rect.w -= sx * 2;
	rect.h -= sy * 2;

	return rect;
}

wRect wRectGrow(wRect rect, float amount)
{
	rect.x -= amount;
	rect.y -= amount;
	rect.w += amount;
	rect.h += amount;

	return rect;
}

bool wRectContains(wRect rect, float x, float y)
{
	if (x < rect.x)
		return false;

	if (x > rect.x + rect.w)
		return false;

	if (y < rect.y)
		return false;

	if (y > rect.y + rect.h)
		return false;

	return true;
}

wRectI wRectIIntersect(wRectI a, wRectI b)
{
	int x1 = (a.x > b.x) ? a.x : b.x;
	int y1 = (a.y > b.y) ? a.y : b.y;
	int x2 = ((a.x + a.w) < (b.x + b.w)) ? (a.x + a.w) : (b.x + b.w);
	int y2 = ((a.y + a.h) < (b.y + b.h)) ? (a.y + a.h) : (b.y + b.h);

	wRectI res = { x1, y1, x2 - x1, y2 - y1 };

	if (res.w < 0 || res.h < 0) {
		wRectI empty = {0, 0, 0, 0};
		return empty;
	}

	return res;
}

void wMat4Ortho(wMat4 *m, float l, float r, float t, float b, float n, float f)
{
	memset(m, 0x0, sizeof(wMat4));

	m->m[0][0] = 2.0f / (r - l);
	m->m[1][1] = 2.0f / (t - b);
	m->m[2][2] = -2.0f / (f - n);
	m->m[3][3] = 1.0f;

	m->m[3][0] = -(r + l) / (r - l);
	m->m[3][1] = -(t + b) / (t - b);
	m->m[3][2] = -(f + n) / (f - n);
}

void wMat4Zero(wMat4 *m)
{
	memset(m, 0x0, sizeof(wMat4));
}

void wMat4Identity(wMat4 *m)
{
	memset(m, 0x0, sizeof(wMat4));

	m->m[0][0] = 1.0f;
	m->m[1][1] = 1.0f;
	m->m[2][2] = 1.0f;
	m->m[3][3] = 1.0f;
}

void wMat4Transpose(wMat4 *m)
{
	for (int i = 0; i < 4; ++i) {
		for (int k = i + 1; k <  4; ++k) {
			int tmp = m->m[i][k];
			m->m[i][k] = m->m[k][i];
			m->m[k][i] = tmp;
		}
	}
}

void wMat4Translate(wMat4 *m, float x, float y, float z)
{
	wMat4Identity(m);

	m->m[0][3] = x;
	m->m[1][3] = y;
	m->m[2][3] = z;
}

void wMat4Multiply(const wMat4 *a, const wMat4 *b, wMat4 *ret)
{
	wMat4Zero(ret);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				ret->m[i][j] += a->m[i][k] * b->m[k][j];
			}
		}
	}	
}

wVec4 wMat4MultiplyPoint(const wMat4 *m, wVec4 v)
{
	wVec4 res = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 4; ++i) {
		res.x += m->m[i][0] * v.x;
		res.y += m->m[i][1] * v.y;
		res.z += m->m[i][2] * v.z;
		res.w += m->m[i][3] * v.w;
	}

	return res;
}
