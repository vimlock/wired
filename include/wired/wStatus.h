#pragma once

typedef struct _wStatus wStatus;
typedef struct _wString wString;
typedef struct _wEntity wEntity;

int wStatusRegister(const wString *name, const wString *script);
int wStatusInvokeOnInit(wStatus *s);
int wStatusInvokeOnThink(wStatus *s);
int wStatusInvokeOnRemoved(wStatus *s);
int wStatusInvokeOnApplied(wStatus *s);
