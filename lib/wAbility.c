#include "../include/wired/wGame.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"

void wAbilityRegister(const wString *name, const wString *script)
{
	wAssert(name != NULL);
	wAssert(script != NULL);
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
