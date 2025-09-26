#pragma once

#include "wMath.h"
#include "wString.h"
#include "wBuffer.h"
#include "wPainter.h"
#include "wShader.h"
#include "wGui.h"
#include "wSprite.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <lua5.4/lauxlib.h>

typedef struct _wClass wClass;
typedef struct _wFont wFont;

typedef struct _wlEnum wlEnum;
struct _wlEnum
{
	const char *name;
	int value;
};

void wlPushVec2(lua_State *L, wVec2 vec);
wVec2 *wlCheckVec2(lua_State *L, int index);
void wlPushIVec2(lua_State *L, wIVec2 vec);
wIVec2 *wlCheckIVec2(lua_State *L, int index);
void wlPushVec3(lua_State *L, wVec3 vec);
wVec3 *wlCheckVec3(lua_State *L, int index);
void wlPushVec4(lua_State *L, wVec4 vec);
wVec4 *wlCheckVec4(lua_State *L, int index);
void wlPushRect(lua_State *L, wRect rect);
wRect *wlCheckRect(lua_State *L, int index);
void wlPushMat4(lua_State *L, const wMat4 *m);
wMat4 * wlCheckMat4(lua_State *L, int index);
void wlPushColor(lua_State *L, wColor col);
wColor * wlCheckColor(lua_State *L, int index);

void wlCheckString(lua_State *L, int index, wString *ret);
void wlPushString(lua_State *L, wString *str);

wBuffer *wlCheckBuffer(lua_State *L, int index);
void wlPushBuffer(lua_State *L, wBuffer *buffer);
void wlRegisterBuffer(lua_State *L);

wImage *wlCheckImage(lua_State *L, int index);
void wlPushImage(lua_State *L, wImage *img);

wShader * wlCheckShader(lua_State *L, int index);
void wlPushShader(lua_State *L, wShader *shader);

wTexture *wlCheckTexture(lua_State *L, int index);
void wlPushTexture(lua_State *L, wTexture *tex);
void wlRegisterTexture(lua_State *L);

wPainter *wlCheckPainter(lua_State *L, int index);
void wlPushPainter(lua_State *L, wPainter *painter);

wSpriteAnim *wlCheckSpriteAnim(lua_State *L, int index);
void wlPushSpriteAnim(lua_State *L, wSpriteAnim *anim);

wFont *wlCheckFont(lua_State *L, int index);
void wlPushFont(lua_State *L, wFont *font);

void wlDumpStack();

void wlRegisterFunc(lua_State *L, const char *name, lua_CFunction func);
void wlRegisterType(lua_State *L, const char *name, const luaL_Reg *reg);
void wlRegisterDerivedType(lua_State *L, const char *name, const char *base, const luaL_Reg *reg);
void wlRegisterEnum(lua_State *L, const char *name, const  wlEnum *e);

void wlRegisterFile(lua_State *L);
void wlRegisterFont(lua_State *L);
void wlRegisterGui(lua_State *L);
void wlRegisterGuiButton(lua_State *L);
void wlRegisterImage(lua_State *L);
void wlRegisterLog(lua_State *L);
void wlRegisterMath(lua_State *L);
void wlRegisterPainter(lua_State *L);
void wlRegisterShader(lua_State *L);
void wlRegisterSpriteAnim(lua_State *L);
void wlRegisterTexture(lua_State *L);
void wlRegisterWindow(lua_State *L);
void wlRegisterTileLayer(lua_State *L);

int wlCheckMethod(lua_State *L);
void *wlCheckClass(lua_State *L, int index, const char *name);
void wlPushClass(lua_State *L, void *obj, const char *cls);

#ifdef __cplusplus
}
#endif
