#include "../include/wired/wEvent.h"
#include "../include/wired/wWrap.h"

static const wlEnum wlKey[] =
{
	{ "KEY_A", W_KEY_A },
	{ "KEY_B", W_KEY_B },
	{ "KEY_C", W_KEY_C },
	{ "KEY_D", W_KEY_D },
	{ "KEY_E", W_KEY_E },
	{ "KEY_F", W_KEY_F },
	{ "KEY_G", W_KEY_G },
	{ "KEY_H", W_KEY_H },
	{ "KEY_I", W_KEY_I },
	{ "KEY_J", W_KEY_J },
	{ "KEY_K", W_KEY_K },
	{ "KEY_L", W_KEY_L },
	{ "KEY_M", W_KEY_M },
	{ "KEY_N", W_KEY_N },
	{ "KEY_O", W_KEY_O },
	{ "KEY_P", W_KEY_P },
	{ "KEY_Q", W_KEY_Q },
	{ "KEY_R", W_KEY_R },
	{ "KEY_S", W_KEY_S },
	{ "KEY_T", W_KEY_T },
	{ "KEY_U", W_KEY_U },
	{ "KEY_V", W_KEY_V },
	{ "KEY_W", W_KEY_W },
	{ "KEY_X", W_KEY_X },
	{ "KEY_Y", W_KEY_Y },
	{ "KEY_Z", W_KEY_Z },
	{ NULL, 0 },

};

static const wlEnum wlEventType[] =
{
	{ "EVENT_QUIT", W_EVENT_QUIT },
	{ "EVENT_MOUSE_PRESS", W_EVENT_MOUSE_PRESS },
	{ "EVENT_MOUSE_RELEASE", W_EVENT_MOUSE_RELEASE },
	{ "EVENT_MOUSE_MOVE",    W_EVENT_MOUSE_MOVE },
	{ "EVENT_MOUSE_WHEEL",   W_EVENT_MOUSE_WHEEL },

	{ "EVENT_KEY_PRESS",     W_EVENT_KEY_PRESS },
	{ "EVENT_KEY_RELEASE",   W_EVENT_KEY_RELEASE},

	{ NULL, 0 },
};

static const wlEnum wlMouseButton[] =
{
	{ "MOUSE_1", W_MOUSE_1 },
	{ "MOUSE_2", W_MOUSE_2 },
	{ "MOUSE_3", W_MOUSE_3 },
	{ NULL, 0 }
};

void wlOpenEvent(lua_State *L)
{
	wlRegisterEnum(L, "MouseButton", wlMouseButton);
	wlRegisterEnum(L, "Key", wlKey);
	wlRegisterEnum(L, "EventType", wlEventType);
}
