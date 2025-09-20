#include "../include/wired/wWrap.h"

wTexture *wlCheckTexture(lua_State *L, int index)
{
	// TODO
	return 0;
}

void wlPushTexture(lua_State *L, wTexture *tex)
{
	// TODO
}

static int wlTextureGetSize(lua_State *L)
{
	wTexture *self = wlCheckTexture(L, 1);
	wlPushIVec2(L, wTextureGetSize(self));
	return 1;
}

static luaL_Reg wlTexture[] = {
	{ "GetSize", wlTextureGetSize },
	{ NULL, NULL }
};

void wlRegisterTexture(lua_State *L)
{
	wlRegisterType(L, "Texture", wlTexture);
}
