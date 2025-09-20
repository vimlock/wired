#include "../include/wired/wWrap.h"

wImage *wlCheckImage(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Image");
}

void wlPushImage(lua_State *L, wImage *img)
{
}

static int wlImage__new(lua_State *L)
{
	return 1;
}

static int wlImage__gc(lua_State *L)
{
	return 0;
}

static int wlImageCopy(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wImage *copy;

	wlPushImage(L, copy);
	return 1;
}

static int wlImageCrop(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wRect *rect = wlCheckRect(L, 2);
	wImage *ret;

	wlPushImage(L, ret);
	return 1;
}

static luaL_Reg wlImage[] = {
	{ "__new", wlImage__new },
	{ "__gc",  wlImage__gc },
	{ "Copy",  wlImageCopy },
	{ "Crop",  wlImageCrop },
};

void wlRegisterImage(lua_State *L)
{
	wlRegisterType(L, "Image", wlImage );
}
