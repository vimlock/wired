#include "../include/wired/wTileMap.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wWrap.h"

void wlPushTileLayer(lua_State *L, wTileLayer *layer)
{
	wlPushClass(L, layer, "TileLayer");
}

wTileLayer * wlCheckTileLayer(lua_State *L, int index)
{
	return wlCheckClass(L, index, "TileLayer");
}

static int wlTileLayer__new(lua_State *L)
{
	int w = luaL_checkinteger(L, 1);
	int h = luaL_checkinteger(L, 2);

	wTileLayer *layer = wTileLayerAlloc(w, h);
	wlPushTileLayer(L, layer);
	return 1;
}

static int wlTileLayer__gc(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	wTileLayerFree(self);
	return 0;
}

static int wlTileLayerLoadRegion(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);

	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int w = luaL_checkinteger(L, 4);
	int h = luaL_checkinteger(L, 5);

	wTileLayerLoadRegion(self, x, y, w, h);
	return 0;
}

static int wlTileLayerDraw(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	wPainter *painter = wlCheckPainter(L, 2);
	wTileLayerDraw(self, painter);
	return 0;
}

static int wlTileLayerSetTile(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int index = luaL_checkinteger(L, 4);
	wTileLayerSetTile(self, x, y, index);
	return 0;
}

static int wlTileLayerSetTileFlags(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int flags = luaL_checkinteger(L, 4);
	wTileLayerSetTileFlags(self, x, y, flags);
	return 0;
}

static int wlTileLayerGetTile(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	wTileIndex index = wTileLayerGetTile(self, x, y);
	lua_pushinteger(L, index);
	return 1;
}

static int wlTileLayerSetSheet(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	luaL_checktype(L, 2, LUA_TTABLE);
	
	size_t len = lua_rawlen(L, 2);
	wRect *items = wMemAlloc(sizeof(wRect) * len);
	int n = 0;

	for (size_t i = 0; i < len; ++i) {
		lua_rawgeti(L, 2, i + 1);
		wRect *rect = wlCheckRect(L, -1);
		items[n++] = *rect;
		lua_pop(L, 1);
	}

	wTileLayerSetSheet(self, len, items);

	wMemFree(items);

	return 0;
}

int wlTileLayerFill(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);

	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int w = luaL_checkinteger(L, 4);
	int h = luaL_checkinteger(L, 5);
	int index = luaL_checkinteger(L, 6);

	wTileLayerFill(self, x, y, w, h, index);

	return 0;
}

int wlTileLayerFillRandom(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);

	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int w = luaL_checkinteger(L, 4);
	int h = luaL_checkinteger(L, 5);
	int index = luaL_checkinteger(L, 6);
	float chance = luaL_checknumber(L, 7);

	wTileLayerFillRandom(self, x, y, w, h, index, chance);

	return 0;
}

static int wlTileLayerSetTexture(lua_State *L)
{
	wTileLayer *self = wlCheckTileLayer(L, 1);
	wString texture; wlCheckString(L, 2, &texture);
	wTileLayerSetTexture(self, &texture);
	return 0;
}

static const luaL_Reg wlTileLayer[] =
{
	{ "__new", wlTileLayer__new },
	{ "__gc",  wlTileLayer__gc },
	{ "SetTile", wlTileLayerSetTile },
	{ "GetTile", wlTileLayerGetTile },
	{ "SetTileFlags", wlTileLayerSetTileFlags },
	{ "LoadRegion", wlTileLayerLoadRegion },
	{ "Draw", wlTileLayerDraw },
	{ "Fill", wlTileLayerFill },
	{ "FillRandom", wlTileLayerFillRandom },
	{ "SetSheet", wlTileLayerSetSheet },
	{ "SetTexture", wlTileLayerSetTexture },

	{ NULL, NULL }
};

void wlRegisterTileLayer(lua_State *L)
{
	wlRegisterType(L, "TileLayer", wlTileLayer);
}
