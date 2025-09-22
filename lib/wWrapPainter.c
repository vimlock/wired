#include "../include/wired/wWrap.h"
#include "../include/wired/wError.h"

wPainter *wlCheckPainter(lua_State *L, int index)
{
	wPainter **self = luaL_checkudata(L, index, "Painter");
	if (!self)
		return NULL;

	return *self;
}

void wlPushPainter(lua_State *L, wPainter *painter)
{
	wPainter **self = lua_newuserdata(L, sizeof(wPainter*));
	*self = painter;

	luaL_getmetatable(L, "Painter");
	lua_setmetatable(L, -2);
}

static int wlPainter__new(lua_State *L)
{
	int err;
	wPainter *self;

	self = wPainterAlloc();
	err = wPainterInit(self);
	if (err) {
		wPainterFree(self);
		luaL_error(L, "%s", wErrorStr(err));
	}

	wlPushPainter(L, self);
	return 1;
}

static int wlPainter__gc(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wPainterFree(self);
	return 0;
}

static int wlPainterDrawRect(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wRect *rect = wlCheckRect(L, 2);
	wPainterDrawRect(self, *rect);
	return 0;
}

static int wlPainterDrawTexture(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wRect *rect = wlCheckRect(L, 2);
	wTexture *tex = wlCheckTexture(L, 3);
	wPainterDrawTexture(self, *rect, tex);
	return 0;
}

static int wlPainterDrawImage(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wRect *rect = wlCheckRect(L, 2);
	wImage *img = wlCheckImage(L, 3);
	wPainterDrawImage(self, *rect, img);
	return 0;
}

static int wlPainterClear(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wColor *col = wlCheckColor(L, 2);
	wPainterClear(self, *col);
	return 0;
}

static int wlPainterPushState(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wPainterPushState(self);
	return 0;
}

static int wlPainterPopState(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wPainterPopState(self);
	return 0;
}

static int wlPainterTranslate(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float z = luaL_checknumber(L, 4);

	wPainterTranslate(self, x, y, z);
	return 0;
}

static int wlPainterSetShader(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wShader *shader = wlCheckShader(L, 2);

	wPainterSetShader(self, shader);

	return 0;
}

static int wlPainterSetColor(lua_State *L)
{
	wPainter *self = wlCheckPainter(L, 1);
	wColor *col = wlCheckColor(L, 2);

	wPainterSetColor(self, *col);
	return 0;
}

static int wlPainterSetViewport(lua_State *L)
{
	wRectI viewport;
	wPainter *self = wlCheckPainter(L, 1);
	viewport.x = luaL_checkinteger(L, 2);
	viewport.y = luaL_checkinteger(L, 3);
	viewport.w = luaL_checkinteger(L, 4);
	viewport.h = luaL_checkinteger(L, 5);

	wPainterSetViewport(self, viewport);
	return 0;
}

static int wlPainterSetScissor(lua_State *L)
{
	wRectI scissor;
	wPainter *self = wlCheckPainter(L, 1);
	scissor.x = luaL_checkinteger(L, 2);
	scissor.y = luaL_checkinteger(L, 3);
	scissor.w = luaL_checkinteger(L, 4);
	scissor.h = luaL_checkinteger(L, 5);

	wPainterSetScissor(self, scissor);
	return 0;
}

static luaL_Reg wlPainter[] = {

	{ "__new",       wlPainter__new },
	{ "__gc",        wlPainter__gc },
	{ "DrawRect",    wlPainterDrawRect },
	{ "DrawTexture", wlPainterDrawTexture },
	{ "DrawImage",   wlPainterDrawImage },
	{ "Clear",       wlPainterClear },
	{ "PushState",   wlPainterPushState },
	{ "PopState",    wlPainterPopState },
	{ "Translate",   wlPainterTranslate },
	{ "SetShader",   wlPainterSetShader },
	{ "SetColor",    wlPainterSetColor },
	{ "SetViewport", wlPainterSetViewport },
	{ "SetScissor",  wlPainterSetScissor },
	{ NULL, NULL }
};

void wlRegisterPainter(lua_State *L)
{
	wlRegisterType(L, "Painter", wlPainter);
}
