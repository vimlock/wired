#include "../include/wired/wWrap.h"
#include "../include/wired/wPlatform.h"

static int wlWindowOpen(lua_State *L)
{
	// TODO
	return 0;
}

static int wlWindowClose(lua_State *L)
{
	// TODO
	return 0;
}

static int wlWindowSetIcon(lua_State *L)
{
	// TODO
	return 0;
}

static int wlWindowSetTitle(lua_State *L)
{
	// TODO
	return 0;
}

static int wlWindowSwapBuffers(lua_State *L)
{
	// TODO
	return 0;
}

static int wlApplicationShouldQuit(lua_State *L)
{
	lua_pushboolean(L, wApplicationShouldQuit());
	return 1;
}

static int wlApplicationQuit(lua_State *L)
{
	wApplicationQuit();
	return 0;
}

static luaL_Reg wlApplication[] = {
	{ "ShouldQuit", wlApplicationShouldQuit },
	{ "Quit", wlApplicationQuit },
	{ NULL, NULL },
};

static luaL_Reg wlWindow[] = {
	{ "Open",  wlWindowOpen },
	{ "Close", wlWindowClose },
	{ "SetTitle", wlWindowSetTitle },
	{ "SwapBuffers", wlWindowSwapBuffers },
	{ NULL, NULL },
};

void wlRegisterWindow(lua_State *L)
{
	wlRegisterType(L, "Application", wlApplication);
	wlRegisterType(L, "Window", wlWindow);
}
