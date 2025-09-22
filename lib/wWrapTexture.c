#include "../include/wired/wTexture.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wWrap.h"

wTexture *wlCheckTexture(lua_State *L, int index)
{
	wTexture **self = luaL_checkudata(L, index, "Texture");
	if (!self)
		return NULL;

	wAssert(*self != NULL);
	return *self;
}

void wlPushTexture(lua_State *L, wTexture *tex)
{
	wTexture **self = lua_newuserdata(L, sizeof(wTexture*));
	*self = tex;

	luaL_getmetatable(L, "Texture");
	lua_setmetatable(L, -2);
}

static int wlTextureGetSize(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	wlPushIVec2(L, wTextureGetSize(self));
	return 1;
}

static int wlTexture__new(lua_State *L)
{
	wTexture *tex = wTextureAlloc();
	wlPushTexture(L, tex);
	return 1;
}

static int wlTexture__gc(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	wTextureFree(self);
	return 0;
}

static int wlTextureLoadFromImage(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	wImage *img = wlCheckImage(L, 2);
	wTextureLoadFromImage(self, img);
	return 0;
}	

static int wlTextureGenMipMaps(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	wTextureGenMipMaps(self);
	return 0;
}

static int wlTextureSetFilter(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	int mode = luaL_checkinteger(L, 2);
	wTextureSetFilter(self, mode);
	return 0;
}

static int wlTextureSetWrap(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	int mode = luaL_checkinteger(L, 2);
	wTextureSetWrap(self, mode);
	return 0;
}

static luaL_Reg wlTexture[] = {
	{ "__new",     wlTexture__new },
	{ "__gc",      wlTexture__gc },
	{ "LoadFromImage", wlTextureLoadFromImage },
	{ "GenMipMaps", wlTextureGenMipMaps },
	{ "GetSize",   wlTextureGetSize },
	{ "SetFilter", wlTextureSetFilter },
	{ "SetWrap",   wlTextureSetWrap },
	{ NULL, NULL }
};

void wlRegisterTexture(lua_State *L)
{
	wlRegisterType(L, "Texture", wlTexture);
}
