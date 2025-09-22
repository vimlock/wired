#pragma once

#include "wBuffer.h"
#include "wMath.h"
#include "wString.h"

#include <stdint.h>

typedef struct _wImage wImage;
typedef struct _wTexture wTexture;
typedef struct _wSpriteSheet wSpriteSheet;
typedef struct _wSpriteAnim wSpriteAnim;
typedef struct _wShader wShader;

typedef uintptr_t wNativeHandle;

enum wShaderValueType
{
	W_SHADER_FLOAT,
	W_SHADER_VEC2,
	W_SHADER_VEC3,
	W_SHADER_VEC4,
	W_SHADER_MAT4,
};


/* ---------- Window ---------- */
int wWindowOpen(int width, int height);
void wWindowClose();
int wWindowSetIcon(const wImage *img);
int wWindowSetTitle(const wString *title);
int wWindowSetFullscreen();
int wWindowMaximize();
int wWindowMinimize();
int wWindowGetWidth();
int wWindowGetHeight();
int wWindowPollEvents();
int wWindowSwapBuffers();

/* ---------- Screen ---------- */
int wScreenGetCount();
int wScreenGetX(int screen);
int wScreenGetY(int screen);
int wScreenGetWidth(int screen);
int wScreenGetHeight(int screen);
void wScreenGetName(int screen, wString *ret);

/* ---------- Cursor ---------- */
void wCursorShow();
void wCursorHide();

/* ---------- SpriteSheet ---------- */

int wSpriteAnimLoad(wSpriteAnim *anim, const wString *path);
int wSpriteAnimSave(wSpriteAnim *anim, const wString *path);
int wSpriteAnimGetFrameCount(wSpriteAnim *anim);
int wSpriteAnimAppendFrame(wSpriteAnim *anim, wRect rect);
wRect wSpriteAnimGetFrameRect(wSpriteAnim *anim, int frame);
int wSpriteAnimGetAction(wSpriteAnim *anim);
int wSpriteAnimSetAction(wSpriteAnim *anim, int frame);
int wSpriteAnimGetRecovery(wSpriteAnim *anim);
int wSpriteAnimSetRecovery(wSpriteAnim *anim, int frame);
int wSpriteAnimGetCanInterrupt(wSpriteAnim *anim);
int wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt);

int wSpriteSheetLoad(wSpriteAnim *anim, const wString *path);
int wSpriteSheetSave(wSpriteAnim *anim, const wString *path);
int wSpriteSheetSetDefault(wSpriteAnim *anim, const wString *state);
int wSpriteSheetAddState(wSpriteAnim *anim, const wString *state, const wString *sheet);
int wSpriteSheetSetFallback(wSpriteAnim *anim, const wString *state, const wString *fallback);
int wSpriteSheetGetStateCount(wSpriteAnim *anim);
