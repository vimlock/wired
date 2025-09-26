#pragma once

#include "wMath.h"

typedef struct _wSpriteSheet wSpriteSheet;
typedef struct _wSpriteAnim wSpriteAnim;
typedef struct _wString wString;

wSpriteAnim * wSpriteAnimAlloc();
void wSpriteAnimFree(wSpriteAnim *anim);
wSpriteAnim * wSpriteAnimCopy(const wSpriteAnim *src);

int wSpriteAnimLoad(wSpriteAnim *anim, const wString *path);
int wSpriteAnimSave(wSpriteAnim *anim, const wString *path);

int wSpriteAnimGetFrameCount(const wSpriteAnim *anim);

void wSpriteAnimAppendFrame(wSpriteAnim *anim, int ticks, wRect rect);
wRect wSpriteAnimGetFrameRect(wSpriteAnim *anim, int frame);

int wSpriteAnimGetAction(const wSpriteAnim *anim);
void wSpriteAnimSetAction(wSpriteAnim *anim, int frame);

int wSpriteAnimGetRecovery(const wSpriteAnim *anim);
void wSpriteAnimSetRecovery(wSpriteAnim *anim, int frame);

int wSpriteAnimGetCanInterrupt(const wSpriteAnim *anim);
void wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt);

wSpriteSheet *wSpriteSheetAlloc();
void wSpriteSheetFree(wSpriteSheet *sheet);
int wSpriteSheetLoad(wSpriteSheet *sheet, wSpriteAnim *anim, const wString *path);
int wSpriteSheetSave(wSpriteSheet *sheet, wSpriteAnim *anim, const wString *path);
int wSpriteSheetSetDefault(wSpriteAnim *anim, const wString *state);
int wSpriteSheetAddState(wSpriteAnim *anim, const wString *state, const wString *sheet);
int wSpriteSheetSetFallback(wSpriteAnim *anim, const wString *state, const wString *fallback);
int wSpriteSheetGetStateCount(wSpriteAnim *anim);
