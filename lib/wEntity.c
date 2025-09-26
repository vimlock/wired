#include "../include/wired/wEntity.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wError.h"
#include "../include/wired/wString.h"
#include "../include/wired/wMemory.h"

#include <stddef.h>

static wClass wEntityClass =
{
	.name = "Entity",
	.base = NULL,
	.version = 1,
};

struct _wEntity
{
	wClass *cls;
	wString *name;

	wAbility **ability;
	int abilityCount;

	wStatus **status;
	int statusCount;

	wItem **item;
	int itemCount;

	wSpriteEffect **effects;
	int effectsCount;

	wSpriteParticles **particles;
	int particlesCount;
};

wEntity *wEntityAlloc()
{
	wEntity *ret = wMemAlloc(sizeof(wEntity));

	ret->cls = &wEntityClass;
	ret->name = wStringFromCString("");

	return ret;
}

void wEntityFree(wEntity *e)
{
	if (!e)
		return;

	wStringFree(e->name);

	wMemFree(e->ability);
	wMemFree(e->status);
	wMemFree(e->effects);
	wMemFree(e->particles);

	wMemFree(e);
}

int wEntityReduceMana(const wEntity *entity)
{
	wAssert(entity != NULL);

	return W_NOT_IMPLEMENTED;
}

int wEntityReduceHealth(const wEntity *entity)
{
	wAssert(entity != NULL);

	return W_NOT_IMPLEMENTED;
}

int wEntityAddSpriteEffect(const wEntity *entity, const wString *name, const wStatus *source)
{
	wAssert(entity != NULL);

	return W_NOT_IMPLEMENTED;
}

int wEntityAddSpriteParticles(const wEntity *entity, const wString *name, const wStatus *source)
{
	wAssert(entity != NULL);

	return W_NOT_IMPLEMENTED;
}

int wEntityGetAbilityCount(wEntity *e)
{
	return e->abilityCount;
}

wAbility *wEntityGetAbility(wEntity *entity, int index)
{
	wAssert(entity != NULL);

	return NULL;
}

int wEntityGetStatusCount(wEntity *entity)
{
	wAssert(entity != NULL);

	return entity->statusCount;
}

wStatus *wEntityGetStatus(wEntity *entity, int index)
{
	wAssert(entity != NULL);

	return NULL;
}

void wEntityAddStatus(const wEntity *entity, const wString *name, int duration, const wEntity *source)
{
	wAssert(entity != NULL);
}

int wEntityGetItemCount(wEntity *entity)
{
	wAssert(entity != NULL);
	return entity->itemCount;
}

wItem *wEntityGetItem(wEntity *entity, int index)
{
	wAssert(entity != NULL);
	return NULL;
}
