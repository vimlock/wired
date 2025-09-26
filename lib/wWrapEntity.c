#include "../include/wired/wWrap.h"
#include "../include/wired/wEntity.h"

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
