#include "../include/wired/wWrap.h"

#include <string.h>

/* ------------- wVec2 ------------------- */

void wlPushVec2(lua_State *L, wVec2 vec)
{
	wVec2 *self = lua_newuserdata(L, sizeof(wVec2));
	*self = vec;

	luaL_getmetatable(L, "Vec2");
	lua_setmetatable(L, -2);
}

wVec2 * wlCheckVec2(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Vec2");
}

static int wlVec2__new(lua_State *L)
{
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);

	wVec2 vec = { x, y };
	wlPushVec2(L, vec);
	return 1;
}

static int wlVec2__index(lua_State *L)
{
	if (wlCheckMethod(L))
		return 1;

	wVec2 *vec = wlCheckVec2(L, 1);
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "x") == 0)
		lua_pushnumber(L, vec->x);
	else if (strcmp(key, "y") == 0)
		lua_pushnumber(L, vec->y);
	else
		luaL_error(L, "invalid property '%s'", key);

	return 1;
}

static int wlVec2__newindex(lua_State *L)
{
	wVec2 *vec = wlCheckVec2(L, 1);
	const char *key = luaL_checkstring(L, 2);
	float val = luaL_checknumber(L, 3);

	if (strcmp(key, "x") == 0)
		vec->x = val;
	else if (strcmp(key, "y") == 0)
		vec->y = val;
	else
		luaL_error(L, "invalid property '%s'", key);

	return 0;
}

static int wlVec2__tostring(lua_State *L)
{
	wVec2 *vec = wlCheckVec2(L, 1);
	// TODO
	lua_pushstring(L, "Vec2");
	return 1;
}

/* ------------- wIVec2 ------------------- */

void wlPushIVec2(lua_State *L, wIVec2 vec)
{
	wIVec2 *self = lua_newuserdata(L, sizeof(wIVec2));
	*self = vec;

	luaL_getmetatable(L, "IVec2");
	lua_setmetatable(L, -2);
}

wIVec2 * wlCheckIVec2(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "IVec2");
}

static int wlIVec2__new(lua_State *L)
{
	float x = luaL_checkinteger(L, 1);
	float y = luaL_checkinteger(L, 2);

	wIVec2 vec = { x, y };
	wlPushIVec2(L, vec);
	return 1;
}

static int wlIVec2__index(lua_State *L)
{
	if (wlCheckMethod(L))
		return 1;

	wIVec2 *vec = wlCheckIVec2(L, 1);
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "x") == 0)
		lua_pushinteger(L, vec->x);
	else if (strcmp(key, "y") == 0)
		lua_pushinteger(L, vec->y);
	else
		luaL_error(L, "invalid property '%s'", key);

	return 1;
}

static int wlIVec2__newindex(lua_State *L)
{
	wIVec2 *vec = wlCheckIVec2(L, 1);
	const char *key = luaL_checkstring(L, 2);
	int val = luaL_checkinteger(L, 3);

	if (strcmp(key, "x") == 0)
		vec->x = val;
	else if (strcmp(key, "y") == 0)
		vec->y = val;
	else
		luaL_error(L, "invalid property '%s'", key);

	return 0;
}

static int wlIVec2__tostring(lua_State *L)
{
	wIVec2 *vec = wlCheckIVec2(L, 1);
	// TODO
	return 0;
}

/* ------------- wVec3 ------------------- */

void wlPushVec3(lua_State *L, wVec3 vec)
{
	wVec3 *self = lua_newuserdata(L, sizeof(wVec3));
	*self = vec;

	luaL_getmetatable(L, "Vec3");
	lua_setmetatable(L, -2);
}

wVec3 * wlCheckVec3(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Vec3");
}

static int wlVec3__new(lua_State *L)
{
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);

	wVec3 vec = { x, y, z };
	wlPushVec3(L, vec);
	return 1;
}

static int wlVec3__index(lua_State *L)
{
	if (wlCheckMethod(L))
		return 1;

	wVec3 *vec = wlCheckVec3(L, 1);
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "x") == 0)
		lua_pushnumber(L, vec->x);
	else if (strcmp(key, "y") == 0)
		lua_pushnumber(L, vec->y);
	else if (strcmp(key, "z") == 0)
		lua_pushnumber(L, vec->z);
	else
		luaL_error(L, "invalid property '%s'", key);

	return 1;
}

static int wlVec3__newindex(lua_State *L)
{
	wVec3 *vec = wlCheckVec3(L, 1);
	const char *key = luaL_checkstring(L, 2);
	float val = luaL_checknumber(L, 3);

	if (strcmp(key, "x") == 0)
		vec->x = val;
	else if (strcmp(key, "y") == 0)
		vec->y = val;
	else if (strcmp(key, "z") == 0)
		vec->z = val;
	else
		luaL_error(L, "invalid property '%s'", key);

	return 0;
}

static int wlVec3__tostring(lua_State *L)
{
	wVec4 *self = wlCheckVec4(L, 1);
	lua_pushfstring(L, "{ x=%f, y=%f, z=%f }", self->x, self->y, self->z);
	return 1;
}

luaL_Reg wlVec3[] = {
	{ "__new",   wlVec3__new },
	{ "__index", wlVec3__index },
	{ "__newindex", wlVec3__newindex },
	{ "__tostring", wlVec3__tostring },
	{ NULL, NULL },
};

/* ------------- wVec4 ------------------- */

void wlPushVec4(lua_State *L, wVec4 vec)
{
	wVec4 *self = lua_newuserdata(L, sizeof(wVec4));
	*self = vec;

	luaL_getmetatable(L, "Vec4");
	lua_setmetatable(L, -2);
}

wVec4 * wlCheckVec4(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Vec4");
}

static int wlVec4__new(lua_State *L)
{
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);
	float w = luaL_checknumber(L, 4);

	wVec4 vec = { x, y, z, w };
	wlPushVec4(L, vec);
	return 1;
}

static int wlVec4__index(lua_State *L)
{
	if (wlCheckMethod(L))
		return 1;

	wVec4 *vec = wlCheckVec4(L, 1);
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "x") == 0)
		lua_pushnumber(L, vec->x);
	else if (strcmp(key, "y") == 0)
		lua_pushnumber(L, vec->y);
	else if (strcmp(key, "z") == 0)
		lua_pushnumber(L, vec->z);
	else if (strcmp(key, "w") == 0)
		lua_pushnumber(L, vec->w);
	else
		return 0;

	return 1;
}

static int wlVec4__newindex(lua_State *L)
{
	wVec4 *vec = wlCheckVec4(L, 1);
	const char *key = luaL_checkstring(L, 2);
	float val = luaL_checknumber(L, 3);

	if (strcmp(key, "x") == 0)
		vec->x = val;
	else if (strcmp(key, "y") == 0)
		vec->y = val;
	else if (strcmp(key, "z") == 0)
		vec->z = val;
	else if (strcmp(key, "w") == 0)
		vec->w = val;
	else
		luaL_error(L, "invalid property '%s'", key);

	return 1;
}

static int wlVec4__tostring(lua_State *L)
{
	wVec4 *self = wlCheckVec4(L, 1);
	lua_pushfstring(L, "{ x=%f, y=%f, z=%f, w=%f}", self->x, self->y, self->z, self->w);
	return 1;
}

/* ------------- wMat4 ------------------- */

void wlPushMat4(lua_State *L, const wMat4 *m)
{
	wMat4 *self = lua_newuserdata(L, sizeof(wMat4));
	*self = *m;

	luaL_getmetatable(L, "Mat4");
	lua_setmetatable(L, -2);
}

wMat4 * wlCheckMat4(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Mat4");
}

static int wlMat4__new(lua_State *L)
{
	wMat4 m;
	wMat4Identity(&m);
	wlPushMat4(L, &m);
	return 1;
}

static int wlMat4Ortho(lua_State *L)
{
	float l = luaL_checknumber(L, 1);
	float r = luaL_checknumber(L, 2);
	float t = luaL_checknumber(L, 3);
	float b = luaL_checknumber(L, 4);
	float n = luaL_checknumber(L, 5);
	float f = luaL_checknumber(L, 6);

	wMat4 m;
	wMat4Ortho(&m, l, r, t, b, n, f);
	wlPushMat4(L, &m);
	return 1;
}

static int wlMat4Identity(lua_State *L)
{
	wMat4 m;
	wMat4Identity(&m);
	wlPushMat4(L, &m);
	return 1;
}

static int wlMat4Translate(lua_State *L)
{
	// TODO
	return 1;
}

static int wlMat4Multiply(lua_State *L)
{
	wMat4 *a = wlCheckMat4(L, 1);
	wMat4 *b = wlCheckMat4(L, 2);
	wMat4 r;
	wMat4Multiply(a, b, &r);
	wlPushMat4(L, &r);
	return 1;
}

static int wlMat4__tostring(lua_State *L)
{
	wRect *self = wlCheckRect(L, 1);
	lua_pushfstring(L, "Mat4");
	return 1;
}


/* ------------- wRect ---------------- */

void wlPushRect(lua_State *L, wRect rect)
{
	wRect *self = lua_newuserdata(L, sizeof(wRect));
	*self = rect;

	luaL_getmetatable(L, "Rect");
	lua_setmetatable(L, -2);
}

wRect *wlCheckRect(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Rect");
}

static int wlRect__new(lua_State *L)
{
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float w = luaL_checknumber(L, 3);
	float h = luaL_checknumber(L, 4);

	wRect rect = { x, y, w, h };
	wlPushRect(L, rect);
	return 1;
}

static int wlRect__index(lua_State *L)
{
	if (wlCheckMethod(L))
		return 1;

	wRect *self = wlCheckRect(L, 1);
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "x") == 0)
		lua_pushnumber(L, self->x);
	else if (strcmp(key, "y") == 0)
		lua_pushnumber(L, self->y);
	else if (strcmp(key, "w") == 0)
		lua_pushnumber(L, self->w);
	else if (strcmp(key, "h") == 0)
		lua_pushnumber(L, self->h);
	else
		return 0;

	return 1;
}

static int wlRect__newindex(lua_State *L)
{
	wRect *self = wlCheckRect(L, 1);
	const char *key = luaL_checkstring(L, 2);
	float val = luaL_checknumber(L, 3);

	if (strcmp(key, "x") == 0)
		self->x = val;
	else if (strcmp(key, "y") == 0)
		self->y = val;
	else if (strcmp(key, "w") == 0)
		self->w = val;
	else if (strcmp(key, "h") == 0)
		self->h = val;
	else
		luaL_error(L, "Invalid property '%s'", key);

	return 0;
}

static int wlRectShrink(lua_State *L)
{
	wRect *self = wlCheckRect(L, 1);
	float amount = luaL_checknumber(L, 2);
	wlPushRect(L, wRectShrink(*self, amount));
	return 1;
}

static int wlRectGrow(lua_State *L)
{
	wRect *self = wlCheckRect(L, 1);
	float amount = luaL_checknumber(L, 2);
	wlPushRect(L, wRectGrow(*self, amount));
	return 1;
}

static int wlRect__tostring(lua_State *L)
{
	wRect *self = wlCheckRect(L, 1);
	lua_pushfstring(L, "{ w=%f, y=%f, w=%f, h=%f }", self->x, self->y, self->w, self->h);
	return 1;
}

/* ------------- wColor ------------------- */

void wlPushColor(lua_State *L, wColor col)
{
	wColor *self = lua_newuserdata(L, sizeof(wColor));
	*self = col;

	luaL_getmetatable(L, "Color");
	lua_setmetatable(L, -2);
}

wColor * wlCheckColor(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "Color");
}

static int wlColor__new(lua_State *L)
{
	float r = luaL_checknumber(L, 1);
	float g = luaL_checknumber(L, 2);
	float b = luaL_checknumber(L, 3);
	float a = luaL_checknumber(L, 4);

	wColor col = { r, g, b, a };
	wlPushColor(L, col);

	return 1;
}

static int wlColor__index(lua_State *L)
{
	if (wlCheckMethod(L))
		return 1;

	wColor *self = wlCheckColor(L, 1);
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "r") == 0)
		lua_pushnumber(L, self->r);
	else if (strcmp(key, "g") == 0)
		lua_pushnumber(L, self->g);
	else if (strcmp(key, "b") == 0)
		lua_pushnumber(L, self->b);
	else if (strcmp(key, "a") == 0)
		lua_pushnumber(L, self->a);
	else
		return 0;

	return 1;
}

static int wlColor__newindex(lua_State *L)
{
	wColor *self = wlCheckColor(L, 1);
	const char *key = luaL_checkstring(L, 2);
	float val = luaL_checknumber(L, 3);

	if (strcmp(key, "r") == 0)
		self->r = val;
	else if (strcmp(key, "g") == 0)
		self->g = val;
	else if (strcmp(key, "b") == 0)
		self->b = val;
	else if (strcmp(key, "a") == 0)
		self->a = val;
	else
		luaL_error(L, "Invalid property '%s'", key);

	return 0;
}

static int wlColor__tostring(lua_State *L)
{
	wColor *self = wlCheckColor(L, 1);
	lua_pushfstring(L, "{ r=%f, g=%f, b=%f, a=%f }", self->r, self->g, self->b, self->a);
	return 1;
}

luaL_Reg wlVec2[] = {
	{ "__new",   wlVec2__new },
	{ "__index", wlVec2__index },
	{ "__newindex", wlVec2__newindex },
	{ "__tostring", wlVec2__tostring },
	{ NULL, NULL },
};

static luaL_Reg wlIVec2[] = {
	{ "__new",   wlIVec2__new },
	{ "__index", wlIVec2__index },
	{ "__newindex", wlIVec2__newindex },
	{ "__tostring", wlIVec2__tostring },
	{ NULL, NULL },
};

luaL_Reg wlVec4[] = {
	{ "__new",   wlVec4__new },
	{ "__index", wlVec4__index },
	{ "__newindex", wlVec4__newindex },
	{ "__tostring", wlVec4__tostring },
	{ NULL, NULL },
};

static luaL_Reg wlMat4[] = {
	{ "__new", wlMat4__new },
	{ "__tostring", wlMat4__tostring },
	{ "Identity", wlMat4Identity },
	{ "Ortho", wlMat4Ortho },
	{ "Multiply", wlMat4Multiply },
	{ "Translate", wlMat4Translate },
	{ NULL, NULL },
};


static luaL_Reg wlRect[] = {
	{ "__new", wlRect__new },
	{ "__index", wlRect__index },
	{ "__newindex", wlRect__newindex },
	{ "__tostring", wlRect__tostring },
	{ "Shrink", wlRectShrink },
	{ "Grow", wlRectGrow },
	{ NULL, NULL },
};

static luaL_Reg wlColor[] = {
	{ "__new", wlColor__new },
	{ "__index", wlColor__index },
	{ "__newindex", wlColor__newindex },
	{ "__tostring", wlColor__tostring },
	{ NULL, NULL },
};

void wlRegisterMath(lua_State *L)
{
	wlRegisterType(L, "Rect", wlRect);
	wlRegisterType(L, "Vec2", wlVec2);
	wlRegisterType(L, "Vec3", wlVec3);
	wlRegisterType(L, "Vec4", wlVec4);
	wlRegisterType(L, "Mat4", wlMat4);
	wlRegisterType(L, "Color", wlColor);
}
