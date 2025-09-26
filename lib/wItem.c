#include "../include/wired/wItem.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"

#include <stddef.h>

static const wClass wItemClass =
{
	.name = "Item",
	.base = NULL,
	.version = 1,
};

struct _wItem
{
	const wClass *cls;
	wString *name;
	wString *script;
};

void wItemRegister(const wString *name, const wString *script)
{
}

int wItemUseTargetUnit(wItem *item, wEntity *target)
{
	wAssert(item != NULL);
	wAssert(target != NULL);

	if (!item || !target)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wItemDrop(wItem *item, wIVec2 tile)
{
	wAssert(item != NULL);
	
	if (!item)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wItemInvokeOnInit(wItem *item)
{
	wAssert(item != NULL);

	if (!item)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wItemInvokeOnEquip(wItem *item)
{
	wAssert(item != NULL);

	if (!item)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wItemInvokeOnUnequip(wItem *item)
{
	wAssert(item != NULL);

	if (!item)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}

int wItemInvokeOnThink(wItem *item)
{
	wAssert(item != NULL);

	if (!item)
		return W_INVALID_ARGUMENT;

	return W_NOT_IMPLEMENTED;
}
