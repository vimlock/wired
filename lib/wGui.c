#include "../include/wired/wGui.h"
#include "../include/wired/wGraphics.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"

#include <string.h>

static void wGuiNode_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);

	for (int i = 0; i < wGuiGetNumChildren(self); ++i)
		wGuiNodePaint(wGuiGetChild(self, i), painter);
}

wGuiNode *wGuiNodeAlloc()
{
	wGuiNode *ret;

	ret = wMemAlloc(sizeof(wGuiNode));
	memset(ret, 0x0, sizeof(wGuiNode));

	ret->visible = true;
	ret->paint = wGuiNode_paint;

	return ret;
}

void wGuiNodeFree(wGuiNode *node)
{
	wMemFree(node);
}

wGuiNode * wGuiNodeCreateChild(wGuiNode *parent)
{
	return NULL;
}

void wGuiNodePaint(wGuiNode *node, wPainter *painter)
{
	if (!node->visible)
		return;

	if (!node->paint)
		return;

	wPainterPushState(painter);
	wPainterTranslate(painter, node->rect.x, node->rect.y, 0.0f);
	node->paint(node, painter);
	wPainterPopState(painter);
}

void wGuiUpdateLayout(wGuiNode *node)
{
	if (!node->visible)
		return;

	if (!node->layout)
		return;

	node->layout(node);
}

int wGuiGetNumChildren(wGuiNode *node)
{
	return node->numChildren;
}

wGuiNode *wGuiGetChild(wGuiNode *node, int child)
{
	if (child < 0 || child >= node->numChildren)
		return NULL;

	return node->children[child];
}

void wGuiNodeDestroy(wGuiNode *node)
{
	wAssert(node != NULL);
}

wRect wGuiGetGeometry(wGuiNode *node)
{
	wAssert(node != NULL);
	return node->rect;
}

void wGuiSetGeometry(wGuiNode *node, wRect rect)
{
	wAssert(node != NULL);
	node->rect = rect;
}

void wGuiSetVisible(wGuiNode *node, bool visible)
{
	wAssert(node != NULL);
	node->visible = visible;
}

bool wGuiIsVisible(wGuiNode *node)
{
	wAssert(node != NULL);
	return node->visible;
}

/* --------- Image --------- */

static void wGuiImage_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wPainterDrawImage(painter, wGuiGetGeometry(self), self->image);
	wGuiNode_paint(self, painter);
}

wGuiNode *wGuiImage(wGuiNode *parent)
{
	wGuiNode *ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_IMAGE;
	ret->paint = wGuiImage_paint;

	return ret;
}

/* --------- Button --------- */

static void wGuiButton_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wPainterDrawRect(painter, wGuiGetGeometry(self));
	wGuiNode_paint(self, painter);
}

static void wGuiButton_mouseEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

static void wGuiButton_keyboardEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiButton(wGuiNode *parent)
{
	wGuiNode *ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_BUTTON;
	ret->paint = wGuiButton_paint;
	ret->mouseEvent = wGuiButton_mouseEvent;
	ret->keyboardEvent = wGuiButton_keyboardEvent;

	return ret;
}

/* --------- Label --------- */

static void wGuiLabel_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wPainterDrawImage(painter, wGuiGetGeometry(self), self->image);
}

wGuiNode *wGuiLabel(wGuiNode *parent)
{
	wGuiNode *ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_LABEL;
	ret->paint = wGuiLabel_paint;
	return ret;
}

/* --------- Slider --------- */

static void wGuiSlider_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);

	wRect rect = wGuiGetGeometry(self);

	wRect handle = { 0, 0, 16, rect.h};
	wRect gutter = { 0, 0, rect.w, 8 };

	wPainterDrawRect(painter, gutter);
	wPainterDrawRect(painter, handle);
}

static void wGuiSlider_mouseEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

static void wGuiSlider_keyboardEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiSlider(wGuiNode *parent)
{
	wGuiNode *ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_SLIDER;
	ret->paint = wGuiSlider_paint;
	ret->mouseEvent = wGuiSlider_mouseEvent;
	ret->keyboardEvent = wGuiSlider_keyboardEvent;

	return ret;
}

/* --------- ScrollArea --------- */

static void wGuiScrollArea_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
}

static void wGuiScrollArea_mouseEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiScrollArea(wGuiNode *parent)
{
	wGuiNode *ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_SCROLL;
	ret->paint = wGuiScrollArea_paint;
	ret->mouseEvent = wGuiScrollArea_mouseEvent;

	return ret;
}

/* --------- VBox --------- */

static void wGuiVBox_layout(wGuiNode *self)
{
	wAssert(self != NULL);

	wRect rect = wGuiGetGeometry(self);
}

wGuiNode *wGuiVBox(wGuiNode *parent)
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc();
	if (!ret)
		return ret;

	ret->type = W_GUI_VBOX;
	ret->layout = wGuiVBox_layout;

	return ret;
}

/* --------- HBox --------- */

static void wGuiHBox_layout(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiHBox(wGuiNode *parent)
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc();
	if (!ret)
		return ret;

	ret->type = W_GUI_HBOX;
	ret->layout = wGuiHBox_layout;

	return ret;
}

/* --------- Grid --------- */

static void wGuiGrid_layout(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiGrid(wGuiNode *parent)
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_GRID;
	ret->layout = wGuiGrid_layout;

	return ret;
}

/* --------- Script --------- */

static void wGuiScript_paint(wGuiNode *self, wPainter *painter)
{
}

static void wGuiScript_layout(wGuiNode *self)
{
}

static void wGuiScript_mouseEvent(wGuiNode *self)
{
}

wGuiNode *wGuiScriptNode(wGuiNode *parent)
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc();
	if (!ret)
		return NULL;

	ret->type = W_GUI_SCRIPT;
	ret->paint = wGuiScript_paint;
	ret->layout = wGuiScript_layout;
	ret->mouseEvent = wGuiScript_mouseEvent;

	return ret;
}
