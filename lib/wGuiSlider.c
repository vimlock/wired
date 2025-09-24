#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

static const wClass wGuiSliderClass =
{
	.name = "GuiSlider",
	.base = &wGuiNodeClass,
	.version = 1
};

typedef struct _wGuiSliderPriv
{
	int min;
	int max;
} wGuiSliderPriv;

static void wGuiSlider_free(wGuiNode *self)
{
	wAssert(self != NULL);
}

static void wGuiSlider_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wAssert(painter != NULL);

	wGuiStyle *style = self->style;
	wRect rect = wGuiNodeGetGeometry(self);

	wRect handle = { 0, 0, style->sliderHandleSize, rect.h};
	wRect gutter = { 0, 0, rect.w, style->sliderGutterSize };

	wPainterDrawRect(painter, gutter);
	wPainterDrawRect(painter, handle);
}

static void wGuiSlider_mousePress(wGuiNode *self, float x, float y, int button)
{
	wAssert(self != NULL);
}

static void wGuiSlider_keyboardEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiSlider()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiSliderPriv));
	ret->class = &wGuiSliderClass;
	ret->paint = wGuiSlider_paint;
	ret->free = wGuiSlider_free;
	ret->mousePress   = wGuiSlider_mousePress;
	ret->keyboardEvent = wGuiSlider_keyboardEvent;

	return ret;
}
