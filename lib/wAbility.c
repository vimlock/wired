#include "../include/wired/wAbility.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wString.h"
#include "../include/wired/wMemory.h"

#include <stddef.h>

static const wClass wAbilityClass =
{
	.name = "Ability",
	.base = NULL,
	.version = 1,
};

struct _wAbility
{
	const wClass *cls;
	wString *name;
	wString *script;
};

void wAbilityRegister(const wString *name, const wString *script)
{
	wAssert(name != NULL);
	wAssert(script != NULL);
}

wAbility *wAbilityAlloc()
{
	wAbility *ret = wMemAlloc(sizeof(wAbility));
	ret->name = wStringFromCString("");
	ret->script = wStringFromCString("");
	return ret;
}

void wAbilityFree(wAbility *a)
{
	if (!a)
		return;

	wStringFree(a->name);
	wStringFree(a->script);
	wMemFree(a);
}

int wAbilityUseTargetSelf(wAbility *ability)
{
	wAssert(ability != NULL);
	if (!ability)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wAbilityUseTargetUnit(wAbility *ability, wEntity *target)
{
	wAssert(ability != NULL);
	wAssert(target != NULL);

	if (!ability || !target)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wAbilityUseTargetTile(wAbility *ability, wIVec2 tile)
{
	wAssert(ability != NULL);

	if (!ability)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wAbilityInvokeOnInit(wAbility *ability)
{
	wAssert(ability != NULL);

	if (!ability)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wAbilityInvokeOnFree(wAbility *ability)
{
	wAssert(ability != NULL);

	if (!ability)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}
