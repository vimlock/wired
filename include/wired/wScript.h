#pragma once

#include "wString.h"

typedef struct _wScript wScript;

void wScriptInit(wScript *script);
void wScriptFree(wScript *script);

int wScriptLoad(const wString *file);
int wScriptCall(wString *script, wString *function);

int wScriptVmInit();
void wScriptVmFree();
