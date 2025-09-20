#pragma once

#include "wMath.h"
#include "wString.h"

typedef struct _wPainter wPainter;
typedef struct _wShader wShader;
typedef struct _wImage wImage;
typedef struct _wTexture wTexture;

wPainter *wPainterAlloc();
int wPainterInit(wPainter *painter);

int wPainterSetShader(wPainter *painter, wShader *shader);

void wPainterFree(wPainter *painter);
void wPainterClear(wPainter *painter, wColor col);
void wPainterDrawRect(wPainter *painter, wRect rect);
void wPainterDrawFilledRect(wPainter *painter, wRect rect);
void wPainterDrawText(wPainter *painter, const wRect *rect, const wString *str);
void wPainterDrawImage(wPainter *painter, wRect rect, wImage *img);
void wPainterDrawTexture(wPainter *painter, wRect rect, wTexture *tex);
void wPainterSetColor(wPainter *painter, wColor col);
wColor wPainterGetColor(wPainter *painter);
void wPainterSetScissor(wPainter *painter, wRectI rect);
wRectI wPainterGetScissor(wPainter *painter);
void wPainterSetViewport(wPainter *painter, wRectI rect);
wRectI wPainterGetViewport(wPainter *painter);
void wPainterPushState(wPainter *painter);
void wPainterPopState(wPainter *painter);
void wPainterSetOrtho(wPainter *painter, float left, float right, float bottom, float top, float near, float far);
void wPainterTranslate(wPainter *painter, float x, float y, float z);
