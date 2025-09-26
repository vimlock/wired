#include "../include/wired/wWrap.h"

wSpriteAnim *wlCheckSpriteAnim(lua_State *L, int index)
{
	return wlCheckClass(L, index, "SpriteAnim");
}

void wlPushSpriteAnim(lua_State *L, wSpriteAnim *anim)
{
	wlPushClass(L, anim, "SpriteAnim");
}

static int wlSpriteAnim__new(lua_State *L)
{
	wSpriteAnim *self = wSpriteAnimAlloc();
	wlPushSpriteAnim(L, self);
	return 1;
}

static int wlSpriteAnim__gc(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	wSpriteAnimFree(self);
	return 1;
}

static int wlSpriteAnimGetFrameCount(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	lua_pushinteger(L, wSpriteAnimGetFrameCount(self));
	return 1;
}

static int wlSpriteAnimGetDuration(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	lua_pushinteger(L, wSpriteAnimGetDuration(self));
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
	int tick = luaL_checkinteger(L, 2);
	const wRect *rect = wlCheckRect(L, 3);

	wSpriteAnimAppendFrame(self, tick, *rect);

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

static int wlSpriteAnimSetLooping(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	int val = luaL_checkinteger(L, 2);
	wSpriteAnimSetLooping(self, val);
	return 0;
}

static int wlSpriteAnimSetCanInterrupt(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	int val = luaL_checkinteger(L, 2);
	wSpriteAnimSetCanInterrupt(self, val);
	return 0;
}

static int wlSpriteAnimGetCanInterrupt(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	lua_pushboolean(L, wSpriteAnimGetCanInterrupt(self));
	return 1;
}

static int wlSpriteAnimSetTexture(lua_State *L)
{
	wSpriteAnim *self = wlCheckSpriteAnim(L, 1);
	wString id; wlCheckString(L, 2, &id);
	wSpriteAnimSetTexture(self, &id);
	return 0;
}

wSpriteSheet *wlCheckSpriteSheet(lua_State *L, int index)
{
	return wlCheckClass(L, index, "SpriteSheet");
}

void wlPushSpriteSheet(lua_State *L, wSpriteSheet *sheet)
{
	wlPushClass(L, sheet, "SpriteSheet");
}

static int wlSpriteSheet__new(lua_State *L)
{
	wSpriteSheet *self = wSpriteSheetAlloc();
	wlPushSpriteSheet(L, self);
	return 1;
}

static int wlSpriteSheet__gc(lua_State *L)
{
	wSpriteSheet *self = wlCheckSpriteSheet(L, 1);
	wSpriteSheetFree(self);
	return 1;
}

static int wlSpriteSheetSetAnim(lua_State *L)
{
	wSpriteSheet *self = wlCheckSpriteSheet(L, 1);
	wString name; wlCheckString(L, 2, &name);
	wSpriteAnim *anim = wlCheckSpriteAnim(L, 3);
	wSpriteSheetSetAnim(self, &name, anim);
	return 0;
}

wSpriteRenderer *wlCheckSpriteRenderer(lua_State *L, int index)
{
	return wlCheckClass(L, index, "SpriteRenderer");
}

void wlPushSpriteRenderer(lua_State *L, wSpriteRenderer *renderer)
{
	wlPushClass(L, renderer, "SpriteRenderer");
}

static int wlSpriteRenderer__new(lua_State *L)
{
	wSpriteRenderer *self = wSpriteRendererAlloc();
	wlPushSpriteRenderer(L, self);
	return 1;
}

static int wlSpriteRenderer__gc(lua_State *L)
{
	wSpriteRenderer *self = wlCheckSpriteRenderer(L, 1);
	wSpriteRendererFree(self);
	return 0;
}

static int wlSpriteRendererUpdate(lua_State *L)
{
	wSpriteRenderer *self = wlCheckSpriteRenderer(L, 1);
	wSpriteRendererUpdate(self);
	return 0;
}

static int wlSpriteRendererDraw(lua_State *L)
{
	wSpriteRenderer *self = wlCheckSpriteRenderer(L, 1);
	wPainter *painter = wlCheckPainter(L, 2);
	wSpriteRendererDraw(self, painter);
	return 0;
}

static int wlSpriteRendererSetSheet(lua_State *L)
{
	wSpriteRenderer *self = wlCheckSpriteRenderer(L, 1);
	wSpriteSheet *sheet = wlCheckSpriteSheet(L, 2);
	wSpriteRendererSetSheet(self, sheet);
	return 0;
}

static int wlSpriteRendererSetAnim(lua_State *L)
{
	wSpriteRenderer *self = wlCheckSpriteRenderer(L, 1);
	wString name; wlCheckString(L, 2, &name);
	wSpriteRendererSetAnim(self, &name);
	return 0;
}

static luaL_Reg wlSpriteAnim[] =
{
	{ "__new",     wlSpriteAnim__new },
	{ "__gc",      wlSpriteAnim__gc },
	{ "GetFrameCount", wlSpriteAnimGetFrameCount },
	{ "SetFrameRect", wlSpriteAnimSetFrameRect },
	{ "GetFrameRect", wlSpriteAnimGetFrameRect },
	{ "GetDuration", wlSpriteAnimGetDuration },
	{ "AppendFrame", wlSpriteAnimAppendFrame },
	{ "SetAction", wlSpriteAnimSetAction },
	{ "GetAction", wlSpriteAnimGetAction },
	{ "SetRecovery", wlSpriteAnimSetRecovery },
	{ "GetRecovery", wlSpriteAnimGetRecovery },
	{ "SetCanInterrupt", wlSpriteAnimSetCanInterrupt },
	{ "GetCanInterrupt", wlSpriteAnimGetCanInterrupt },
	{ "SetLooping", wlSpriteAnimSetLooping },
	{ "SetTexture", wlSpriteAnimSetTexture },
	{ NULL, NULL },
};

static luaL_Reg wlSpriteSheet[] =
{
	{ "__new", wlSpriteSheet__new },
	{ "__gc",  wlSpriteSheet__gc },
	{ "SetAnim", wlSpriteSheetSetAnim },
	{ NULL, NULL },
};

static luaL_Reg wlSpriteRenderer[] =
{
	{ "__new", wlSpriteRenderer__new },
	{ "__gc",  wlSpriteRenderer__gc },
	{ "Draw", wlSpriteRendererDraw },
	{ "Update", wlSpriteRendererUpdate },
	{ "SetSheet", wlSpriteRendererSetSheet },
	{ "SetAnim", wlSpriteRendererSetAnim },
	{ NULL, NULL },
};

void wlRegisterSpriteAnim(lua_State *L)
{
	wlRegisterType(L, "SpriteAnim", wlSpriteAnim);
	wlRegisterType(L, "SpriteSheet", wlSpriteSheet);
	wlRegisterType(L, "SpriteRenderer", wlSpriteRenderer);
}
