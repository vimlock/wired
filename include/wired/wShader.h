#pragma once

#include "wPlatform.h"

typedef struct _wShader wShader;

enum wShaderValueType
{
	W_SHADER_FLOAT,
	W_SHADER_VEC2,
	W_SHADER_VEC3,
	W_SHADER_VEC4,
	W_SHADER_MAT4,
};

wShader *wShaderAlloc();
int wShaderInit(wShader *shader);

int wShaderLoad(wShader *shader, const wString *vert, const wString *frag);
void wShaderFree(wShader *shader);
void wShaderSetVertex(wShader *shader, const wString *source);
void wShaderSetFragment(wShader *shader, const wString *source);
int wShaderSetValue(wShader *shader, int location, int type, const void *value);
int wShaderCompile(wShader *shader);
wNativeHandle wShaderGetNativeHandle(wShader *shader);

int wShaderGetUniformLocation(wShader *shader, const char *name);
