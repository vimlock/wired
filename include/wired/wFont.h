#pragma once

#include "wString.h"

typedef struct _wFont wFont;

wFont *wFontAlloc();
int wFontLoad(wFont *font,  const wString *path);
void wFontFree(wFont *font);

void wFontRender(wFont *font, const wString *text);

