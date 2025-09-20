#include "../include/wired/wGame.h"

#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <lua5.4/lauxlib.h>

static void *wlCheckHandle(lua_State *L, int index)
{
	if (!lua_islightuserdata(L, index)) {
		luaL_error(L, "Expecting lightuserdata");
		return 0;
	}

	return lua_touserdata(L, index);
}

/* ------------- wEntity ------------------- */

static int wlEntityReduceHealth(lua_State *L)
{
	return 0;
}

static int wlEntityReduceMana(lua_State *L)
{
	return 0;
}

static int wlEntityGetAbilityCount(lua_State *L)
{
	return 0;
}

static int wlEntityGetAbility(lua_State *L)
{
	return 0;
}

static int wlEntityGetStatusCount(lua_State *L)
{
	return 0;
}

static int wlEntityGetStatus(lua_State *L)
{
	return 0;
}

static int wlEntityGetItemCount(lua_State *L)
{
	return 0;
}

static int wlEntityGetItem(lua_State *L)
{
	return 0;
}

luaL_Reg wlEntity[] = {
	{ "ReduceHealth", wlEntityReduceHealth },
	{ "ReduceMana", wlEntityReduceMana },

	{ "GetAbilityCount", wlEntityGetAbilityCount},
	{ "GetAbility", wlEntityGetAbility },
	{ "GetStatusCount", wlEntityGetStatusCount },
	{ "GetStatus", wlEntityGetStatus },
	{ "GetItemCount", wlEntityGetItemCount },
	{ "GetItem", wlEntityGetItem },
	{ NULL, NULL }
};

/* ------------- wStatus ------------------- */

static int wlStatusGetCaster(lua_State *L)
{
	wStatus *self = wlCheckHandle(L, 1);
	lua_pushlightuserdata(L, self->caster);
	return 1;
}

static int wlStatusGetAbility(lua_State *L)
{
	wStatus *self = wlCheckHandle(L, 1);
	lua_pushlightuserdata(L, self->ability);
	return 1;
}

luaL_Reg funcs[] = {
	{ "GetCaster", wlStatusGetCaster },
	{ "GetAbility", wlStatusGetAbility },
	{ NULL, NULL }
};
