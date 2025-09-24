#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

static const wClass wGuiScrollAreaClass =
{
	.name = "GuiScrollAreaClass",
	.base = &wGuiNodeClass,
	.version = 1
};

struct _wGuiScrollPriv
{
	bool scrollX;
	bool scrollY;
} wGuiScrollPriv;

static void wGuiScrollArea_free(wGuiNode *self)
{
	wAssert(self != NULL);
}

static void wGuiScrollArea_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
}

static void wGuiScrollArea_mouseMove(wGuiNode *self, float x, float y)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiScrollArea()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiScrollPriv));
	if (!ret)
		return NULL;

	ret->class = &wGuiScrollAreaClass;
	ret->paint = wGuiScrollArea_paint;
	ret->mouseMove = wGuiScrollArea_mouseMove;
	ret->free = wGuiScrollArea_free;

	return ret;
}
