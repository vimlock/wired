#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

static const wClass wGuiGridClass =
{
	.name = "GuiGrid",
	.base = &wGuiNodeClass,
	.version = 1
};

struct _wGuiGridPriv
{
	int cols;
} wGuiGridPriv;

static void wGuiGrid_layout(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiGrid()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(sizeof(wGuiGridPriv));
	ret->class = &wGuiGridClass;
	ret->layout = wGuiGrid_layout;

	return ret;
}
