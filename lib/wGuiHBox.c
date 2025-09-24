#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

static const wClass wGuiHBoxClass =
{
	.name = "GuiHBox",
	.base = &wGuiNodeClass,
	.version = 1
};

static void wGuiHBox_layout(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiHBox()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(0);
	ret->class = &wGuiHBoxClass;
	ret->layout = wGuiHBox_layout;

	return ret;
}
