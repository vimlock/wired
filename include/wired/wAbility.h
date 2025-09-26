#pragma once

#include "wMath.h"

typedef struct _wAbility wAbility;
typedef struct _wString wString;
typedef struct _wEntity wEntity;

void wAbilityRegister(const wString *name, const wString *script);
int wAbilityUseTargetSelf(wAbility *a);
int wAbilityUseTargetUnit(wAbility *a, wEntity *target);
int wAbilityUseTargetTile(wAbility *a, wIVec2 tile);
int wAbilityInvokeOnInit(wAbility *a);
int wAbilityInvokeOnFree(wAbility *a);
