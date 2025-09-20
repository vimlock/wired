#pragma once

#include "wGraphics.h"

typedef struct _wShader wShader;

wShader *wShaderAlloc();
int wShaderInit(wShader *shader);

int wShaderLoad(wShader *shader, const wString *vert, const wString *frag);
void wShaderFree(wShader *shader);
int wShaderSetVertex(wShader *shader, const wString *source);
int wShaderSetFragment(wShader *shader, const wString *source);
int wShaderSetValue(wShader *shader, int location, int type, const void *value);
int wShaderCompile(wShader *shader);
wNativeHandle wShaderGetNativeHandle(wShader *shader);

int wShaderGetUniformLocation(wShader *shader, const char *name);
