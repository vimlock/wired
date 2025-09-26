#include "../include/wired/wStatus.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wString.h"

#include <stddef.h>

static const wClass wStatusClass = {
	.name = "Status",
	.base = NULL,
	.version = 1
};

struct _wStatus
{
	const wClass *cls;

	wString *name;
	wString *script;
	wEntity *caster;
};

int wStatusRegister(const wString *name, const wString *script)
{
	return W_NOT_IMPLEMENTED;
}

wStatus *wStatusAlloc()
{
	wStatus *ret = wMemAlloc(sizeof(wStatus));

	ret->cls = &wStatusClass;
	ret->name = wStringFromCString("");
	ret->script = wStringFromCString("");
	ret->caster = NULL;

	return ret;
}

void wStatusFree(wStatus *s)
{
	if (!s)
		return;

	wMemFree(s->name);
	wMemFree(s->script);
	wMemFree(s);
}

int wStatusInvokeOnInit(wStatus *s)
{
	return W_NOT_IMPLEMENTED;
}

int wStatusInvokeOnThink(wStatus *s)
{
	return W_NOT_IMPLEMENTED;
}

int wStatusInvokeOnRemoved(wStatus *s)
{
	return W_NOT_IMPLEMENTED;
}

int wStatusInvokeOnApplied(wStatus *s)
{
	return W_NOT_IMPLEMENTED;
}
