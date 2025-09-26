
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

static const luaL_Reg funcs[] = {
	{ "GetCaster",  wlStatusGetCaster },
	{ "GetAbility", wlStatusGetAbility },
	{ NULL, NULL }
};
