#pragma once

#include "wMath.h"
#include "wString.h"

typedef struct _wAbility wAbility;
typedef struct _wStatus wStatus;
typedef struct _wItem wItem;
typedef struct _wEntity wEntity;
typedef struct _wSpriteEffect wSpriteEffect;
typedef struct _wSpriteParticles wSpriteParticles;

struct _wAbility
{
	wString name;
	wString script;
};

struct _wStatus
{
	wString name;
	wString script;
	wEntity *caster;
};

struct _wSpriteEffect
{
	wStatus *source;
};

struct _wSpriteParticles
{
	wStatus *source;
};

struct _wItem
{
	wString name;
	wString script;
};

struct _wEntity
{
	wString name;

	wAbility *ability;
	int abilityCount;

	wStatus *status;
	int statusCount;

	wItem *item;
	int itemCount;

	wSpriteEffect *effects;
	int effectsCount;

	wSpriteParticles *particles;
	int particlesCount;
};

/* ---------- Map ------------- */

wEntity *wGetEntityInTile(wIVec2 tile);
int wGetEntitiesInRadius(wIVec2 center, int radius, wEntity **ret);

/* ---------- Entity ---------- */

wIVec2 wEntityGetTile(const wEntity *e);

int wEntityReduceMana(const wEntity *e);
int wEntityReduceHealth(const wEntity *e);

int wEntityAddSpriteEffect(const wEntity *e, const wString *name, const wStatus *source);
int wEntityAddSpriteParticles(const wEntity *e, const wString *name, const wStatus *source);

int wEntityGetStatusCount(wEntity *e);
wStatus *wEntityGetStatus(wEntity *e, int index);
void wEntityAddStatus(const wEntity *e, const wString *name, int duration, const wEntity *source);

int wEntityGetAbilityCount(wEntity *e);
wAbility *wEntityGetAbility(wEntity *e, int index);

int wEntityGetItemCount(wEntity *e);
wItem *wEntityGetItem(wEntity *e, int index);

/* ---------- Ability ---------- */

void wAbilityRegister(const wString *name, const wString *script);
int wAbilityUseTargetSelf(wAbility *a);
int wAbilityUseTargetUnit(wAbility *a, wEntity *target);
int wAbilityUseTargetTile(wAbility *a, wIVec2 tile);
int wAbilityInvokeOnInit(wAbility *a);
int wAbilityInvokeOnFree(wAbility *a);

/* ---------- Item ------------- */
void wItemRegister(const wString *name, const wString *script);
int wItemUseTargetUnit(wItem *item, wEntity *target);
int wItemDrop(wItem *item, wIVec2 tile);
int wItemInvokeOnInit(wItem *item);
int wItemInvokeOnEquip(wItem *item);
int wItemInvokeOnUnequip(wItem *item);
int wItemInvokeOnThink(wItem *item);

/* ---------- Status ----------- */
int wStatusRegister(const wString *name, const wString *script);
int wStatusInvokeOnInit(wStatus *s);
int wStatusInvokeOnThink(wStatus *s);
int wStatusInvokeOnRemoved(wStatus *s);
int wStatusInvokeOnApplied(wStatus *s);

/* ---------- Sprite Effect ----- */
void wSpriteEffectRegister(const wString *name);

/* ---------- Sprite Particles -- */
void wSpriteParticlesRegister(const wString *name);
