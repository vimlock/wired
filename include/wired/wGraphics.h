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

enum wTextureFilter
{
	W_TEXTURE_NEAREST,
	W_TEXTURE_BILINEAR,
};

enum wTextureWrap
{
	W_TEXTURE_CLAMP,
	W_TEXTURE_REPEAT,
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

/* ---------- Image ------------ */

int wImageLoad(wImage *img, const wString *name, const wBuffer *data);
int wImageSave(wImage *img, const wString *name, wBuffer *data);
void wImageFill(wImage *img, int r, int g, int b, int a);
int wImageCopy(const wImage *src, wImage *dst);
int wImageCrop(const wImage *src, wImage *dst, wRectI rect);
wTexture *wImageGetTexture(wImage *img);

/* ---------- Texture ---------- */

int wTextureFromImage(wTexture *tex, const wImage *img);
int wTextureGenMipMaps(wTexture *tex);
int wTextureSetFilter(wTexture *tex, int mode);
int wTextureSetWrap(wTexture *tex, int mode);
wNativeHandle wTextureGetNativeHandle(wTexture *tex);
wIVec2 wTextureGetSize(wTexture *tex);

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
