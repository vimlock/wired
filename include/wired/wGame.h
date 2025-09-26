#pragma once

#include <stdint.h>

typedef struct _wPainter wPainter;
typedef struct _wTileLayer wTileLayer;
typedef struct _wGame wGame;
typedef struct _wEntity wEntity;
typedef struct _wString wString;

typedef uint32_t wEntityId;

wGame *wGameAlloc();
void wGameFree(wGame *game);

void wGameDraw(wGame *game, wPainter *painter);

wEntity *wGameGetEntity(wGame *game, wEntityId id);

void wGameRegisterStatus(wGame *game, const wString *name, const wString *script);
void wGameRegisterAbility(wGame *game, const wString *name, const wString *script);
