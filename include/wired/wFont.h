#pragma once

#include "wString.h"
#include "wMath.h"

typedef struct _wFont wFont;

wFont *wFontAlloc();
int wFontLoad(wFont *font,  const wString *path);
void wFontFree(wFont *font);

void wFontRender(wFont *font, const wString *text, wRect rect);
wRect wFontGetRect(wFont *font, const wString *text);
