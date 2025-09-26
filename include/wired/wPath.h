#pragma once

#include "wMath.h"

typedef struct _wPath wPath;

enum wEnterFlags
{
	W_ENTER_TOP          = 1 << 0,
	W_ENTER_LEFT         = 1 << 1,
	W_ENTER_RIGHT        = 1 << 2,
	W_ENTER_BOTTOM       = 1 << 3,
	W_ENTER_TOP_LEFT     = 1 << 4,
	W_ENTER_TOP_RIGHT    = 1 << 5,
	W_ENTER_BOTTOM_LEFT  = 1 << 6,
	W_ENTER_BOTTOM_RIGHT = 1 << 7
};

struct wPathTile
{
	uint8_t enter;
	uint8_t cost;
};

wPath *wPathAlloc();
void wPathFree(wPath *path);

int wPathGetNumWaypoints(const wPath *path);
wIVec2 wPathGetWaypoint(const wPath *path, int index);
void wPathSetWaypoint(wPath *path, int index, wIVec2 tile);

wPath *wPathFind(wPath *path, const wPathTile *tiles, int w, int h);
