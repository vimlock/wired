#pragma once

typedef struct _wItem wItem;
typedef struct _wEntity wEntity;
typedef struct _wString wString;

void wItemRegister(const wString *name, const wString *script);
int wItemUseTargetUnit(wItem *item, wEntity *target);
int wItemInvokeOnInit(wItem *item);
int wItemInvokeOnEquip(wItem *item);
int wItemInvokeOnUnequip(wItem *item);
int wItemInvokeOnThink(wItem *item);
