#pragma once

#include <stdint.h>

#include "wMath.h"

enum wTileFlags
{
	W_TILE_FLAG_NONE   = 0,
	W_TILE_FLAG_FLIP_X = 1 << 0,
	W_TILE_FLAG_FlIP_Y = 1 << 1,
};

typedef struct _wTileData
{
	uint16_t index;
	uint8_t flags;
	uint8_t padding;
} wTileData;

typedef struct _wTileSheet
{
	wRect *tiles;
	unsigned numTiles;
} wTileSheet;

typedef struct _wTileLayer
{
	wTileData *tiles;
	unsigned width;
	unsigned height;
} wTileLayer;

typedef struct _wTileMap
{
	wTileLayer *layers;
	unsigned numLayers;
} wTileMap;
