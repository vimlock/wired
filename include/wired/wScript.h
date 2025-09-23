#pragma once

#include "wString.h"

typedef struct _wScript wScript;

void wScriptInit(wScript *script);
void wScriptFree(wScript *script);

int wScriptLoad(const wString *file);
int wScriptCall(wString *script, wString *function);

int wScriptVmInit();
void wScriptVmFree();

int wScriptCallMousePress(wString *script, float x, float y, int button);
int wScriptCallMouseRelease(wString *script, float x, float y, int button);
int wScriptCallMouseMove(wString *script, float x, float y);
