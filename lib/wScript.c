#include "../include/wired/wScript.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wLog.h"
#include "../include/wired/wWrap.h"

#include <string.h>

struct _wScript
{
	wString filename;
};

static lua_State *L = NULL;

void wlDumpStack()
{
	wLogInfo("==== lua stack ====");
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {
		wLogInfo("[%d] %s", i, luaL_typename(L, i));
	}
}

static int wlTableKeys(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	lua_newtable(L);

	int index = 1;

	lua_pushnil(L);
	while (lua_next(L, 1)) {
		lua_pushvalue(L, -2);
		lua_rawseti(L, 2, index);
		index++;
		lua_pop(L, 1);
	}

	return  1;
}

static int wlTableJoin(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	const char *sep = luaL_optstring(L, 2, ", ");

	size_t len = lua_rawlen(L, 1);
	if (len == 0) {
		lua_pushstring(L, "");
		return 1;
	}

	luaL_Buffer b;
	luaL_buffinit(L, &b);

	for (size_t i = 1; i <= len; ++i) {
		lua_rawgeti(L, 1, i);
		luaL_tolstring(L, -1, NULL);
		luaL_addvalue(&b);
		if (i < len)
			luaL_addstring(&b, sep);
		lua_pop(L, 1);
	}

	luaL_pushresult(&b);
	return 1;
}

static int wlClass(lua_State *L)
{
	lua_newtable(L);
	return 1;
}

void wlRegisterFunc(lua_State *L, const char *name, lua_CFunction func)
{
	lua_pushcfunction(L, func);
	lua_setglobal(L, name);
}

static void wlRegisterMethods(lua_State *L, const char *name, const luaL_Reg *reg)
{
	const luaL_Reg *newf = NULL;

	bool hasIndex = false;

	for (; reg->name; reg++) {

		if (strcmp(reg->name, "__new") == 0) {
			newf = reg;
			continue;
		}

		if (strcmp(reg->name, "__index") == 0)
			hasIndex = true;

		lua_pushcfunction(L, reg->func);
		lua_setfield(L, -2, reg->name);
	}

	if (!hasIndex) {
		lua_pushvalue(L, -1);
		lua_setfield(L, -1, "__index");
	}

	if (newf) {
		lua_pushcfunction(L, newf->func);
		lua_setglobal(L, name);
	}
}

void wlRegisterType(lua_State *L, const char *name, const luaL_Reg *reg)
{
	luaL_newmetatable(L, name);
	wlRegisterMethods(L, name, reg);
	lua_pop(L, 1);
}

void wlRegisterDerivedType(lua_State *L, const char *name, const char* base, const luaL_Reg *reg)
{
	wlRegisterType(L, name, reg);

	luaL_getmetatable(L, name);

	lua_newtable(L);
	luaL_getmetatable(L, base);
	lua_setfield(L, -2, "__index");
	
	lua_setmetatable(L, -2);

	lua_pop(L, 1);
}

int wlCheckMethod(lua_State *L)
{
	if (!lua_getmetatable(L, 1)) {
		luaL_error(L, "no metatable");
		return 0;
	}

	lua_pushvalue(L, 2);
	lua_gettable(L, -2);

	if (!lua_isnil(L, -1)) {
		return 1;
	}

	lua_pop(L, 2);
	return 0;
}

static int wlInitLibs()
{
	wlRegisterFunc(L, "class", wlClass);
	wlRegisterFunc(L, "TableKeys", wlTableKeys);
	wlRegisterFunc(L, "TableJoin", wlTableJoin);

	wlRegisterSpriteAnim(L);
	wlRegisterShader(L);
	wlRegisterMath(L);
	wlRegisterTexture(L);
	wlRegisterBuffer(L);
	wlRegisterPainter(L);
	wlRegisterImage(L);
	wlRegisterLog(L);
	wlRegisterWindow(L);
	wlRegisterGui(L);
	wlRegisterFont(L);
	wlRegisterTileLayer(L);
	wlRegisterCache(L);

	return W_SUCCESS;
}

int wScriptVmInit()
{
	int err;

	L = luaL_newstate();
	if (!L)
		wPanic("Could not allocate Lua state");

	luaL_openlibs(L);

	err = wlInitLibs();
	if (err) {
		lua_close(L);
		L = NULL;
		return err;
	}

	return W_SUCCESS;
}

void wScriptVmFree()
{
	lua_close(L);
	L = NULL;
}

void wScriptInit(wScript *script)
{
	wAssert(script != NULL);
}

void wScriptFree(wScript *script)
{
	wAssert(script != NULL);
}

int wScriptLoad(const wString *script)
{
	wAssert(script != NULL);

	int err;

	/* Check if already cached */
	lua_pushstring(L, wStringData(script));
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (!lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return W_SUCCESS;
	}
	lua_pop(L, 1);

	/* Not loaded, try to load it */
	err = luaL_loadfile(L, wStringData(script));
	if (err) {
		wLogError("Error loading file: %s", lua_tostring(L, -1));
		return W_INVALID_OPERATION;
	}

	wLogDebug("Loaded '%s'", wStringData(script));

	lua_newtable(L);
	lua_pushglobaltable(L);
	lua_setmetatable(L, -2);

	lua_pushstring(L, "_ENV");
	lua_pushvalue(L, -2);
	lua_setupvalue(L, -3, 1);

	lua_pushstring(L, wStringData(script));
	lua_pushvalue(L, -2);
	lua_settable(L, LUA_REGISTRYINDEX);

	lua_pop(L, 3);

	err = lua_pcall(L, 0, 0, 0);
	if (err) {
		wLogError("Error running script: %s", lua_tostring(L, -1));
		return W_INVALID_OPERATION;
	}

	return W_SUCCESS;
}

static int wlScriptPush(wString *script)
{
	wAssert(script != NULL);

	int err;

	lua_pushstring(L, wStringData(script));
	lua_gettable(L, LUA_REGISTRYINDEX);

	/* Not loaded? */
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		wLogError("Script %s is not loaded", wStringData(script));
		return W_INVALID_OPERATION;
	}

	return W_SUCCESS;
}

static int wlScriptDoCall(int numArgs)
{
	int err;

	err = lua_pcall(L, numArgs, 0, 0);
	if (err) {
		wLogWarn("%s", lua_tostring(L, -1));
		return W_INVALID_OPERATION;
	}

	return W_SUCCESS;
}

int wScriptCall(wString *script, wString *function)
{
	wAssert(script != NULL);
	wAssert(wStringSize(script) != 0);
	wAssert(function != NULL);

	int err;

	err = wlScriptPush(script);
	if (err)
		return err;

	lua_getglobal(L, wStringData(function));
	return wlScriptDoCall(0);
}

int wScriptCallMousePress(wString *script, float x, float y, int button)
{
	int err;

	err = wlScriptPush(script);
	if (err)
		return err;

	lua_getglobal(L, "MousePress");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return W_SUCCESS;
	}

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushinteger(L, button);

	return wlScriptDoCall(3);
}

int wScriptCallMouseRelease(wString *script, float x, float y, int button)
{
	int err;

	err = wlScriptPush(script);
	if (err)
		return err;

	lua_getglobal(L, "MouseRelease");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return W_SUCCESS;
	}

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushinteger(L, button);

	return wlScriptDoCall(3);
}

int wScriptCallMouseMove(wString *script, float x, float y)
{
	int err;

	err = wlScriptPush(script);
	if (err)
		return err;

	lua_getglobal(L, "MouseMove");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return W_SUCCESS;
	}

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);

	return wlScriptDoCall(2);
}
