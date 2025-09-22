#include "../include/wired/wWrap.h"
#include "../include/wired/wImage.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"

wImage *wlCheckImage(lua_State *L, int index)
{
	wImage **self = luaL_checkudata(L, index, "Image");
	if (!self)
		return NULL;

	wAssert(*self != NULL);
	return *self;
}

void wlPushImage(lua_State *L, wImage *img)
{
	wImage **self = lua_newuserdata(L, sizeof(wImage*));
	*self = img;

	luaL_getmetatable(L, "Image");
	lua_setmetatable(L, -2);
}

static int wlImage__new(lua_State *L)
{
	wImage *img = wImageAlloc();
	wlPushImage(L, img);
	return 1;
}

static int wlImage__gc(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wImageFree(self);
	return 0;
}

static int wlImageLoad(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wString path;  wlCheckString(L, 2, &path);

	int err = wImageLoad(self, &path);
	lua_pushboolean(L, err == W_SUCCESS);
	return 1;
}	

static int wlImageSave(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wString path;  wlCheckString(L, 2, &path);

	int err = wImageSave(self, &path);
	if (err) {
		luaL_error(L, "%s", wErrorStr(err));
		return 0;
	}

	return 0;
}

static int wlImageFill(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	int r = luaL_checkinteger(L, 2);
	int g = luaL_checkinteger(L, 3);
	int b = luaL_checkinteger(L, 4);
	int a = luaL_checkinteger(L, 5);

	wImageFill(self, r, g, b, a);
	return 0;
}	

static int wlImageCopy(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wImage *copy = wImageCopy(self);
	wlPushImage(L, copy);
	return 1;
}

static int wlImageCrop(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int w = luaL_checkinteger(L, 4);
	int h = luaL_checkinteger(L, 5);
	wRectI rect = {x, y, w, h};
	wImage *ret = wImageCrop(self, rect);

	wlPushImage(L, ret);
	return 1;
}

static int wlImageSize(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wIVec2 size = wImageSize(self);
	wlPushIVec2(L, size);
	return 1;
}	

static int wlImageGetTexture(lua_State *L)
{
	wImage *self = wlCheckImage(L, 1);
	wTexture *tex = wImageGetTexture(self);
	wlPushTexture(L, tex);
	return 1;
}

static luaL_Reg wlImage[] = {
	{ "__new", wlImage__new },
	{ "__gc",  wlImage__gc },
	{ "Load",     wlImageLoad },
	{ "Save",     wlImageSave },
	{ "Fill",     wlImageFill },
	{ "Copy",     wlImageCopy },
	{ "Crop",     wlImageCrop },
	{ "GetSize",  wlImageSize },
	{ "GetTexture", wlImageGetTexture },
	{ NULL, NULL }
};

void wlRegisterImage(lua_State *L)
{
	wlRegisterType(L, "Image", wlImage );
}
