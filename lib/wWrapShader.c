#include "../include/wired/wWrap.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"

void wlPushShader(lua_State *L, wShader *shader)
{
	wShader **self = lua_newuserdata(L, sizeof(wShader*));
	*self = shader;

	luaL_getmetatable(L, "Shader");
	lua_setmetatable(L, -2);
}

wShader * wlCheckShader(lua_State *L, int index)
{
	wShader **self = luaL_checkudata(L, index, "Shader");
	if (!self)
		return NULL;

	wAssert(*self != NULL);

	return *self;
}

static int wlShader__new(lua_State *L)
{
	wShader *self = wShaderAlloc();
	if (!self)
		luaL_error(L, "%s", wErrorStr(W_OUT_OF_MEMORY));

	int err = wShaderInit(self);
	if (err)
		luaL_error(L, "%s", wErrorStr(err));

	wlPushShader(L, self);
	return 1;
}

static int wlShader__gc(lua_State *L)
{
	wShader *self = wlCheckShader(L, 1);
	wShaderFree(self);
	return 0;
}

static int wlShaderSetValue(lua_State *L)
{
	wShader *self = wlCheckShader(L, 1);
	// TODO
	return 0;
}

static int wlShaderSetVertexSource(lua_State *L)
{
	wShader *self;
	wString source;

	self = wlCheckShader(L, 1);
	wlCheckString(L, 2, &source);

	wShaderSetVertex(self, &source);

	return 0;
}

static int wlShaderSetFragmentSource(lua_State *L)
{
	wShader *self;
	wString source;

	self = wlCheckShader(L, 1);
	wlCheckString(L, 2, &source);

	wShaderSetFragment(self, &source);

	return 0;
}

static int wlShaderCompile(lua_State *L)
{
	wShader *self = wlCheckShader(L, 1);
	int err = wShaderCompile(self);
	if (err) {
		luaL_error(L, "%s", wErrorStr(err));
	}

	return 0;
}

static luaL_Reg wlShader[] = {
	{ "__new",     wlShader__new },
	{ "__gc",      wlShader__gc },
	{ "SetValue",  wlShaderSetValue },
	{ "SetVertexSource", wlShaderSetVertexSource },
	{ "SetFragmentSource", wlShaderSetFragmentSource },
	{ "Compile", wlShaderCompile },
	{ NULL, NULL },
};

void wlRegisterShader(lua_State *L)
{
	wlRegisterType(L, "Shader", wlShader);
}

