#pragma once

#include "wMath.h"

typedef struct _wSpriteSheet wSpriteSheet;
typedef struct _wSpriteAnim wSpriteAnim;
typedef struct _wSpriteRenderer wSpriteRenderer;
typedef struct _wString wString;
typedef struct _wPainter wPainter;

/* ------------- SpriteAnim ------------ */

wSpriteAnim * wSpriteAnimAlloc();
void wSpriteAnimFree(wSpriteAnim *anim);
wSpriteAnim * wSpriteAnimCopy(const wSpriteAnim *src);
void wSpriteAnimAssign(wSpriteAnim *dst, const wSpriteAnim *src);

int wSpriteAnimLoad(wSpriteAnim *anim, const wString *path);
int wSpriteAnimSave(wSpriteAnim *anim, const wString *path);

int wSpriteAnimGetFrameCount(const wSpriteAnim *anim);
int wSpriteAnimGetDuration(const wSpriteAnim *anim);

int wSpriteAnimGetFrameIndex(const wSpriteAnim *anim, int time);

void wSpriteAnimAppendFrame(wSpriteAnim *anim, int ticks, wRect rect);
wRect wSpriteAnimGetFrameRect(wSpriteAnim *anim, int frame);

int wSpriteAnimGetAction(const wSpriteAnim *anim);
void wSpriteAnimSetAction(wSpriteAnim *anim, int frame);

int wSpriteAnimGetRecovery(const wSpriteAnim *anim);
void wSpriteAnimSetRecovery(wSpriteAnim *anim, int frame);

int wSpriteAnimGetLooping(const wSpriteAnim *anim);
void wSpriteAnimSetLooping(wSpriteAnim *anim, int looping);

int wSpriteAnimGetCanInterrupt(const wSpriteAnim *anim);
void wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt);

void wSpriteAnimSetTexture(wSpriteAnim *anim, const wString *id);
const wString * wSpriteAnimGetTexture(wSpriteAnim *anim);

/* ------------- SpriteSheet ------------ */

wSpriteSheet *wSpriteSheetAlloc();
void wSpriteSheetFree(wSpriteSheet *sheet);
void wSpriteSheetSetAnim(wSpriteSheet *sheet, const wString *name, const wSpriteAnim *anim);
int wSpriteSheetGetAnimCount(const wSpriteSheet *sheet);
wSpriteAnim *wSpriteSheetGetAnim(const wSpriteSheet *sheet, int index);
wString *wSpriteSheetGetAnimName(const wSpriteSheet *sheet, int index);
wSpriteAnim *wSpriteSheetFindAnim(wSpriteSheet *sheet, const wString *state);

/* ------------- SpriteRenderer ------------ */

wSpriteRenderer *wSpriteRendererAlloc();
void wSpriteRendererFree(wSpriteRenderer *self);

void wSpriteRendererSetSheet(wSpriteRenderer *self, wSpriteSheet *sheet);
void wSpriteRendererSetAnim(wSpriteRenderer *self, const wString *name);
void wSpriteRendererUpdate(wSpriteRenderer *self);
void wSpriteRendererDraw(wSpriteRenderer *self, wPainter *painter);
void wSpriteRendererSetSpeed(wSpriteRenderer *self, float speed);
