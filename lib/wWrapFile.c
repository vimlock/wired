#include "../include/wired/wWrap.h"

#include "../include/wired/wFile.h"
#include "../include/wired/wError.h"

static int wlFileReadAll(lua_State *L)
{
	wString path;
	wBuffer data;
	int err;

	wlCheckString(L, 1, &path);

	err = wFileReadAll(&path, &data);
	if (err) {
		wBufferFree(&data);
		luaL_error(L, "error reading file: %s", wErrorStr(err));
		return 0;
	}

	wlPushBuffer(L, &data);
	return 1;
}

static int wlFileReadAllText(lua_State *L)
{
	wString path;
	wString data;

	int err;

	wlCheckString(L, 1, &path);

	err = wFileReadAllText(&path, &data);
	if (err) {
		wStringFree(&data);
		luaL_error(L, "error reading file: %s", wErrorStr(err));
		return 0;
	}

	wlPushString(L, &data);
	return 1;
}

static int wlFileWriteAll(lua_State *L)
{
	wString path;
	wBuffer *buf;
	int err;

	wlCheckString(L, 1, &path);
	buf = wlCheckBuffer(L, 2);

	err = wFileWriteAll(&path, buf);
	if (err) {
		luaL_error(L, "%s", wErrorStr(err));
	}

	return 0;
}

static int wlFileWriteAllText(lua_State *L)
{
	wString path;
	wString buf;
	int err;

	wlCheckString(L, 1, &path);
	wlCheckString(L, 2, &buf);

	err = wFileWriteAllText(&path, &buf);
	if (err) {
		luaL_error(L, "%s", wErrorStr(err));
	}

	return 0;
}

static luaL_Reg wlFile[] = {
	{ "ReadAll", wlFileReadAll },
	{ "ReadAllText", wlFileReadAllText },
	{ "WriteAll", wlFileWriteAll },
	{ "WriteAllText", wlFileWriteAllText },
	{ NULL, NULL }
};

void wlRegisterFile(lua_State *L)
{
	wlRegisterType(L, "File", wlFile);
}
