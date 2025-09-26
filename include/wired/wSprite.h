#pragma once

#include "wMath.h"

typedef struct _wSpriteSheet wSpriteSheet;
typedef struct _wSpriteAnim wSpriteAnim;
typedef struct _wString wString;

int wSpriteAnimLoad(wSpriteAnim *anim, const wString *path);
int wSpriteAnimSave(wSpriteAnim *anim, const wString *path);

int wSpriteAnimGetFrameCount(wSpriteAnim *anim);

void wSpriteAnimAppendFrame(wSpriteAnim *anim, int ticks, wRect rect);
wRect wSpriteAnimGetFrameRect(wSpriteAnim *anim, int frame);

int wSpriteAnimGetAction(wSpriteAnim *anim);
void wSpriteAnimSetAction(wSpriteAnim *anim, int frame);

int wSpriteAnimGetRecovery(wSpriteAnim *anim);
void wSpriteAnimSetRecovery(wSpriteAnim *anim, int frame);

int wSpriteAnimGetCanInterrupt(wSpriteAnim *anim);
void wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt);

int wSpriteSheetLoad(wSpriteAnim *anim, const wString *path);
int wSpriteSheetSave(wSpriteAnim *anim, const wString *path);
int wSpriteSheetSetDefault(wSpriteAnim *anim, const wString *state);
int wSpriteSheetAddState(wSpriteAnim *anim, const wString *state, const wString *sheet);
int wSpriteSheetSetFallback(wSpriteAnim *anim, const wString *state, const wString *fallback);
int wSpriteSheetGetStateCount(wSpriteAnim *anim);
