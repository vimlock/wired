#include "../include/wired/wWrap.h"
#include "../include/wired/wAssert.h"

void wlCheckString(lua_State *L, int index, wString *ret)
{
	wAssert(ret != NULL);

	size_t size;
	const char *str = luaL_checklstring(L, index, &size);

	ret->data = (char*)str;
	ret->size = size;
	ret->capacity = 0;
}

void wlPushString(lua_State *L, wString *str)
{
	wAssert(str != NULL);

	lua_pushlstring(L, str->data, str->size);
}

