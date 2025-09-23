#pragma once

#include <stdbool.h>

typedef struct _wEvent wEvent;
typedef struct _wKeyEvent wKeyEvent;
typedef struct _wMouseEvent wMouseEvent;
typedef struct _wMouseWheelEvent wMouseWheelEvent;

enum wKey
{
	W_KEY_A,
	W_KEY_B,
	W_KEY_C,
	W_KEY_D,
	W_KEY_E,
	W_KEY_F,
	W_KEY_G,
	W_KEY_H,
	W_KEY_I,
	W_KEY_J,
	W_KEY_K,
	W_KEY_L,
	W_KEY_M,
	W_KEY_N,
	W_KEY_O,
	W_KEY_P,
	W_KEY_Q,
	W_KEY_R,
	W_KEY_S,
	W_KEY_T,
	W_KEY_U,
	W_KEY_V,
	W_KEY_W,
	W_KEY_X,
	W_KEY_Y,
	W_KEY_Z,
};

enum wMouseButton
{
	W_MOUSE_1,
	W_MOUSE_2,
	W_MOUSE_3,
};

enum wEventType
{
	W_EVENT_QUIT,

	W_EVENT_MOUSE_PRESS,
	W_EVENT_MOUSE_MOVE,
	W_EVENT_MOUSE_RELEASE,
	W_EVENT_MOUSE_WHEEL,

	W_EVENT_KEY_PRESS,
	W_EVENT_KEY_RELEASE,
};

struct _wMouseEvent
{
	float x;
	float y;
	int button;
};

struct _wKeyEvent
{
	int key;
};

struct _wMouseWheelEvent
{
	int y;
};

struct _wEvent
{
	int type;
	union {
		wMouseEvent mouse;
		wMouseWheelEvent wheel;
		wKeyEvent key;
	};
};

void wEventInit();
void wEventFree();

void wEventPump();
bool wEventCanPush();
bool wEventPush(const wEvent *evt);
bool wEventPoll(wEvent *ret);
