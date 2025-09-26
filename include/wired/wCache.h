#pragma once

typedef struct _wTexture wTexture;
typedef struct _wImage wImage;
typedef struct _wString wString;

void wLoadTexture(const wString *id, const wString *path);
wTexture *wGetTexture(const wString *id);
