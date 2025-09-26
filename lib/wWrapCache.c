#include "../include/wired/wWrap.h"
#include "../include/wired/wCache.h"

static int wlLoadTexture(lua_State *L)
{
	wString id; wlCheckString(L, 1, &id);
	wString path; wlCheckString(L, 2, &path);
	wLoadTexture(&id, &path);
	return 0;
}

void wlRegisterCache(lua_State *L)
{
	wlRegisterFunc(L, "LoadTexture", wlLoadTexture);
}
