#include "../include/wired/wWrap.h"
#include "../include/wired/wLog.h"

static void coerceString(lua_State *L, int index)
{
	index = lua_absindex(L, index);

	if (lua_isstring(L, index))
		return;

	if (luaL_callmeta(L, index, "__tostring")) {
		if (!lua_isstring(L, -1)) {
			luaL_error(L, "__tostring must return a string");
		}
		lua_replace(L, index);
	}
}

static int wlLogDebug(lua_State *L)
{
	coerceString(L, 1);
	const char *msg = luaL_checklstring(L, 1, NULL);
	wLogDebug("%s", msg);
	return 0;
}

static int wlLogInfo(lua_State *L)
{
	coerceString(L, 1);
	const char *msg = luaL_checklstring(L, 1, NULL);
	wLogInfo("%s", msg);
	return 0;
}

static int wlLogWarn(lua_State *L)
{
	coerceString(L, 1);
	const char *msg = luaL_checklstring(L, 1, NULL);
	wLogWarn("%s", msg);
	return 0;
}

static int wlLogError(lua_State *L)
{
	coerceString(L, 1);
	const char *msg = luaL_checklstring(L, 1, NULL);
	wLogError("%s", msg);
	return 0;
}

static int wlLogFatal(lua_State *L)
{
	coerceString(L, 1);
	const char *msg = luaL_checklstring(L, 1, NULL);
	wLogFatal("%s", msg);
	return 0;
}

void wlRegisterLog(lua_State *L)
{
	wlRegisterFunc(L, "LogDebug", wlLogDebug);
	wlRegisterFunc(L, "LogInfo",  wlLogInfo);
	wlRegisterFunc(L, "LogWarn",  wlLogWarn);
	wlRegisterFunc(L, "LogError", wlLogError);
	wlRegisterFunc(L, "LogFatal", wlLogFatal);
}
