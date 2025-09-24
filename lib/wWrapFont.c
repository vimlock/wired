#include "../include/wired/wWrap.h"
#include "../include/wired/wFont.h"

wFont *wlCheckFont(lua_State *L, int index)
{
	return wlCheckClass(L, index, "Font");
}

void wlPushFont(lua_State *L, wFont *font)
{
	wlPushClass(L, font, "Font");
}

static int wlFont__new(lua_State *L)
{
	wFont *self = wFontAlloc();

	wlPushClass(L, self, "Font");
	return 1;
}

static int wlFont__gc(lua_State *L)
{
	wFont *self = wlCheckFont(L, 1);
	wFontFree(self);
	return 0;
}

static int wlFontLoad(lua_State *L)
{
	wFont *self = wlCheckFont(L, 1);
	wString path; wlCheckString(L, 2, &path);
	wFontLoad(self, &path);
	return 0;
}

static luaL_Reg wlFont[] =
{
	{ "__new", wlFont__new },
	{ "__gc",  wlFont__gc },
	{ "Load",  wlFontLoad },
	{ NULL, NULL },
};

void wlRegisterFont(lua_State *L)
{
	wlRegisterType(L, "Font", wlFont);
}
