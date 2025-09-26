#pragma once

#include <stdint.h>

typedef struct _wTileData wTileData;
typedef struct _wTileSheet wTileSheet;
typedef struct _wTileLayer wTileLayer;
typedef struct _wTileMap wTileMap;
typedef struct _wPainter wPainter;
typedef struct _wRect wRect;
typedef struct _wString wString;

enum wTileFlags
{
	W_TILE_FLAG_NONE   = 0,
	W_TILE_FLAG_FLIP_X = 1 << 0,
	W_TILE_FLAG_FlIP_Y = 1 << 1,
	W_TILE_FLAG_SOLID  = 1 << 2
};

typedef uint16_t wTileIndex;

wTileMap *wTileMapAlloc();
void wTileMapFree(wTileMap *map);
void wTileMapDraw(wTileMap *map);

wTileLayer *wTileLayerAlloc(int width, int height);
void wTileLayerFree(wTileLayer *layer);
void wTileLayerLoadRegion(wTileLayer *layer, int x, int y, int w, int h);
void wTileLayerDraw(wTileLayer *layer, wPainter *painter);
void wTileLayerSetTileFlags(wTileLayer *layer, int x, int y, unsigned flags);
void wTileLayerSetTile(wTileLayer *layer, int x, int y, wTileIndex index);
void wTileLayerFill(wTileLayer *layer, int x, int y, int w, int h, wTileIndex index);
wTileIndex wTileLayerGetTile(wTileLayer *layer, int x, int y);
void wTileLayerSetSheet(wTileLayer *layer, int count, const wRect *rects);
void wTileLayerSetTexture(wTileLayer *layer, const wString *texture);
