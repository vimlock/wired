#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

static const wClass wGuiVBoxClass =
{
	.name = "GuiVBox",
	.base = &wGuiNodeClass,
	.version = 1
};

static void wGuiVBox_layout(wGuiNode *self)
{
	wAssert(self != NULL);

	wRect rect = wGuiNodeGetGeometry(self);
	float hsize = rect.h;

	float minsize = 0.0f;

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self, i);
		minsize += child->minSize.y;
	}

	float h = (hsize - minsize) / wGuiNodeGetNumChildren(self);
	float y = 0.0f;

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self, i);
		wRect crect = { 0, y, rect.w, h };
		wGuiNodeSetGeometry(child, crect);
		y += crect.h;
	}
}

wGuiNode *wGuiVBox()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(0);
	ret->class = &wGuiVBoxClass;
	ret->layout = wGuiVBox_layout;

	return ret;
}
