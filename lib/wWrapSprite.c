#include "../include/wired/wWrap.h"

wSpriteAnim *wlCheckSpriteAnim(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "SpriteAnim");
}

void wlPushSpriteAnim(lua_State *L, wSpriteAnim *anim)
{
}

static int wlSpriteAnim__new(lua_State *L)
{
	return 1;
}

static int wlSpriteAnim__gc(lua_State *L)
{
	return 1;
}

static int wlSpriteAnimGetFrameCount(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	lua_pushinteger(L, wSpriteAnimGetFrameCount(self));
	return 1;
}

static int wlSpriteAnimGetFrameRect(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	int index = luaL_checkinteger(L, 2);
	wRect rect = wSpriteAnimGetFrameRect(self, index);
	wlPushRect(L, rect);
	return 1;
}

static int wlSpriteAnimSetFrameRect(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	int index = luaL_checkinteger(L, 2);
	wRect *rect = wlCheckRect(L, 3);
	return 0;
}

static int wlSpriteAnimAppendFrame(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	wRect *rect = wlCheckRect(L, 2);

	// TODO

	return 0;
}

static int wlSpriteAnimSetRecovery(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	int offset = luaL_checkinteger(L, 2);
	wSpriteAnimSetRecovery(self, offset);
	return 0;
}

static int wlSpriteAnimGetRecovery(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	lua_pushinteger(L, wSpriteAnimGetRecovery(self));
	return 1;
}

static int wlSpriteAnimSetAction(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	int offset = luaL_checkinteger(L, 2);
	wSpriteAnimSetAction(self, offset);
	return 0;
}

static int wlSpriteAnimGetAction(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	lua_pushinteger(L, wSpriteAnimGetAction(self));
	return 1;
}

static int wlSpriteAnimSetCanInterrupt(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	// TODO
	return 0;
}

static int wlSpriteAnimGetCanInterrupt(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	// TODO
	return 0;
}

static luaL_Reg wlSpriteAnim[] = {
	{ "__new",     wlSpriteAnim__new },
	{ "__gc",      wlSpriteAnim__gc },
	{ "GetFrameCount", wlSpriteAnimGetFrameCount },
	{ "SetFrameRect", wlSpriteAnimSetFrameRect },
	{ "GetFrameRect", wlSpriteAnimGetFrameRect },
	{ "AppendFrame", wlSpriteAnimAppendFrame },
	{ "SetAction", wlSpriteAnimSetAction },
	{ "GetAction", wlSpriteAnimGetAction },
	{ "SetRecovery", wlSpriteAnimSetRecovery },
	{ "GetRecovery", wlSpriteAnimGetRecovery },
	{ "SetCanInterrupt", wlSpriteAnimSetCanInterrupt },
	{ "GetCanInterrupt", wlSpriteAnimGetCanInterrupt },
	{ NULL, NULL },
};

void wlRegisterSpriteAnim(lua_State *L)
{
	wlRegisterType(L, "SpriteAnim", wlSpriteAnim);
}
