#pragma once

typedef struct _wEntity wEntity;
typedef struct _wAbility wAbility;
typedef struct _wItem wItem;
typedef struct _wStatus wStatus;
typedef struct _wString wString;
typedef struct _wSpriteEffect wSpriteEffect;
typedef struct _wSpriteParticles wSpriteParticles;

wEntity *wEntityAlloc();
void wEntityFree(wEntity *e);

int wEntityReduceMana(const wEntity *e);
int wEntityReduceHealth(const wEntity *e);

int wEntityGetAbilityCount(wEntity *e);
wAbility *wEntityGetAbility(wEntity *e, int index);

int wEntityGetItemCount(wEntity *e);
wItem *wEntityGetItem(wEntity *e, int index);

int wEntityGetStatusCount(wEntity *e);
wStatus *wEntityGetStatus(wEntity *e, int index);
void wEntityAddStatus(const wEntity *e, const wString *name, int duration, const wEntity *source);

int wEntityAddSpriteEffect(const wEntity *e, const wString *name, const wStatus *source);
int wEntityAddSpriteParticles(const wEntity *e, const wString *name, const wStatus *source);
