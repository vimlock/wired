#include "../include/wired/wTileMap.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wMath.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wPlatform.h"

#include <string.h>

#define MAX_RENDER_W 32
#define MAX_RENDER_H 32
#define MAX_RENDER_TILES (MAX_RENDER_W * MAX_RENDER_H)

struct _wTileData
{
	uint16_t index;
	uint8_t flags;
	uint8_t padding;
};

static const wClass wTileLayerClass =
{
	.name = "TileLayer",
	.base = NULL,
	.version = 1
};

struct _wTileLayer
{
	const wClass *cls;

	wTileData *tiles;
	int width;
	int height;

	float tileSize;

	wPlatformOps *platform;

	wVertex *vertices;
	wIndex *indices;
	int numIndices;

	wNativeHandle vbo;
	wNativeHandle ibo;

	wRect *sheet;
	int sheetCount;
};

wTileLayer *wTileLayerAlloc(int width, int height)
{
	wAssert(width > 0 && height > 0);

	wTileLayer *ret = wMemAlloc(sizeof(wTileLayer));
	ret->cls = &wTileLayerClass;
	ret->platform = wPlatform;
	ret->width = width;
	ret->height = height;
	ret->tileSize = 32;
	ret->tiles = wMemAlloc(sizeof(wTileData) * width * height);

	size_t vs = sizeof(wVertex) * MAX_RENDER_TILES * 4;
	size_t is = sizeof(wIndex) * MAX_RENDER_TILES * 6;

	ret->vbo = ret->platform->bufferCreate(vs, NULL);
	ret->ibo = ret->platform->bufferCreate(is, NULL);

	ret->vertices = wMemAlloc(vs);
	ret->indices = wMemAlloc(is);

	return ret;
}

void wTileLayerFree(wTileLayer *layer)
{
	if (!layer)
		return;

	layer->platform->bufferDestroy(layer->ibo);
	layer->platform->bufferDestroy(layer->vbo);

	wMemFree(layer->tiles);
	wMemFree(layer->vertices);
	wMemFree(layer->indices);
}

void wTileLayerLoadRegion(wTileLayer *layer, int x, int y, int w, int h)
{
	wAssert(layer != NULL);

	int count = 0;

	float ts = layer->tileSize;
	
	wLogDebug("Loading region %d %d %d %d", x, y, w, h);

	for (int i = 0; i < h; ++i) {
		for (int k = 0; k < w; ++k) {
			int oy = i + y;
			int ox = k + x;
			const wTileData *tile = &layer->tiles[oy*layer->width+ox];
			int index = tile->index;
			if (!index)
				continue;

			index -= 1;

			float tx = k * ts;
			float ty = i * ts;

			wRect uv = layer->sheet[tile->index];

			wVertex *vertex = &layer->vertices[count * 4];
			vertex[0].posX = tx;
			vertex[0].posY = ty;
			vertex[0].posZ = 0.0f;
			vertex[0].texX = uv.x;
			vertex[0].texY = uv.y;

			vertex[1].posX = tx + ts;
			vertex[1].posY = ty;
			vertex[1].posZ = 0.0f;
			vertex[1].texX = uv.x + uv.w;
			vertex[1].texY = uv.y;

			vertex[2].posX = tx + ts;
			vertex[2].posY = ty + ts;
			vertex[2].posZ = 0.0f;
			vertex[2].texX = uv.x + uv.w;
			vertex[2].texY = uv.y + uv.h;

			vertex[3].posX = tx;
			vertex[3].posY = ty + ts;
			vertex[3].posZ = 0.0f;
			vertex[3].texX = uv.x;
			vertex[3].texY = uv.y + uv.h;

			wIndex baseIndex = count * 4;
			wIndex *indices = &layer->indices[count * 6];
			indices[0] = baseIndex + 0;
			indices[1] = baseIndex + 1;
			indices[2] = baseIndex + 2;
			indices[3] = baseIndex + 2;
			indices[4] = baseIndex + 3;
			indices[5] = baseIndex + 0;

			wLogDebug("Use tile %f %f %f %f", tx, ty, ts, ts);

			count++;
		}
	}

	layer->platform->bufferData(layer->vbo, count * sizeof(wVertex) * 4, layer->vertices);
	layer->platform->bufferData(layer->ibo, count * sizeof(wIndex) * 6, layer->indices);

	layer->numIndices = count * 6;
}

void wTileLayerDraw(wTileLayer *layer, wPainter *painter)
{
	wPainterBindShader(painter);
	wPainterBindTexture(painter, NULL, 0);
	layer->platform->draw(layer->numIndices, layer->vbo, layer->ibo);
}

int wTileLayerGetSheetSize(wTileLayer *layer)
{
	wAssert(layer != NULL);
	return layer->sheetCount;
}

void wTileLayerSetSheet(wTileLayer *layer, int count, const wRect *rects)
{
	wAssert(layer != NULL);
	layer->sheet = wMemRealloc(layer->sheet, sizeof(wRect) * count);
	memcpy(layer->sheet, rects, sizeof(wRect) * count);
	layer->sheetCount = count;
	wLogDebug("Set sheet %d items", count);
}

void wTileLayerSetTile(wTileLayer *layer, int x, int y, wTileIndex index)
{
	wAssert(layer != NULL);
	wAssert(x >= 0 && y >= 0);
	wAssert(x < layer->width && y < layer->height);
	layer->tiles[y * layer->width + x].index = index;
}

void wTileLayerSetTileFlags(wTileLayer *layer, int x, int y, unsigned flags)
{
	wAssert(layer != NULL);
	wAssert(x >= 0 && y >= 0);
	wAssert(x < layer->width && y < layer->height);
	layer->tiles[y * layer->width + x].flags = flags;
}

wTileIndex wTileLayerGetTile(wTileLayer *layer, int x, int y)
{
	wAssert(x >= 0 && y >= 0);
	wAssert(x < layer->width && y < layer->height);
	return layer->tiles[y * layer->width + x].index;
}

void wTileLayerFill(wTileLayer *layer, int x, int y, int w, int h, wTileIndex index)
{
	wAssert(x >= 0 && y >= 0);
	wAssert(w >= 0 && h >= 0);
	wAssert(x < layer->width && y < layer->height);
	wAssert(x + w < layer->width && y + h < layer->height);

	for (; y < h; ++y) {
		for (; x < w; ++x) {
			wTileLayerSetTile(layer, x, y, index);
		}
	}
}

static const wClass wTileMapClass = {
	.name = "TileMap",
	.base = NULL,
	.version = 1,
};

struct _wTileMap
{
	const wClass *cls;
	wTileLayer *layers;
	unsigned numLayers;
};

wTileMap *wTileMapAlloc()
{
	wTileMap *ret = wMemAlloc(sizeof(wTileMap));
	ret->cls = &wTileMapClass;
	return ret;
}

void wTileMapFree(wTileMap *map)
{
	wMemFree(map);
}
