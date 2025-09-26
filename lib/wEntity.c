#include "../include/wired/wEntity.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"

struct _wEntity
{
	wString *name;

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
