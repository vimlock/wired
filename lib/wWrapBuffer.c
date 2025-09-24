#include "../include/wired/wWrap.h"

wBuffer *wlCheckBuffer(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Buffer");
}

void wlPushBuffer(lua_State *L, wBuffer *buffer)
{
}

static int wlBuffer__new(lua_State *L)
{
	/* TODO: */
	return 0;
}

static int wlBuffer__gc(lua_State *L)
{
	/* TODO: */
	return 0;
}

static int wlBufferClear(lua_State *L)
{
	wBuffer *self = wlCheckBuffer(L, 1);
	wBufferClear(self);
	return 0;
}

static int wlBufferReserve(lua_State *L)
{
	wBuffer *self = wlCheckBuffer(L, 1);
	size_t capacity = luaL_checkinteger(L, 2);
	wBufferReserve(self, capacity);
	return 0;
}

static int wlBufferResize(lua_State *L)
{
	wBuffer *self = wlCheckBuffer(L, 1);
	size_t size = luaL_checkinteger(L, 2);
	wBufferResize(self, size);
	return 0;
}

static luaL_Reg wlBuffer[] = {
	{ "__new", wlBuffer__new },
	{ "__gc",  wlBuffer__gc },

	{ "Clear", wlBufferClear },
	{ "Reserve", wlBufferReserve },
	{ "Resize", wlBufferResize },
	{ NULL, NULL }
};

void wlRegisterBuffer(lua_State *L)
{
	wlRegisterType(L, "Buffer", wlBuffer );
}
