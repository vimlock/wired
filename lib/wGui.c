#include "../include/wired/wGui.h"
#include "../include/wired/wGraphics.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wError.h"
#include "../include/wired/wLog.h"

#include <string.h>

static void wGuiNode_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);

	for (int i = 0; i < wGuiGetNumChildren(self); ++i)
		wGuiNodePaint(wGuiGetChild(self, i), painter);
}

wGuiNode *wGuiNodeAlloc(int privSize)
{
	wGuiNode *ret;

	ret = wMemAlloc(sizeof(wGuiNode) + privSize);
	memset(ret, 0x0, sizeof(wGuiNode) + privSize);

	ret->visible = true;
	ret->paint = wGuiNode_paint;
	ret->priv = ret + 1;

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

int wGuiNodeAddChild(wGuiNode *node, wGuiNode *child)
{
	wGuiNode **newChildren = wMemRealloc(node->children, (node->numChildren + 1) * sizeof(wGuiNode*));
	if (!newChildren)
		return W_OUT_OF_MEMORY;

	node->children = newChildren;
	node->children[node->numChildren++] = child;
	child->parent = node;

	return W_SUCCESS;
}

void wGuiNodePaint(wGuiNode *node, wPainter *painter)
{
	if (!node->visible)
		return;

	if (!node->paint)
		return;

	wPainterPushState(painter);
	// wPainterTranslate(painter, node->rect.x, node->rect.y, 0.0f);
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

	for (int i = 0; i < wGuiGetNumChildren(node); ++i) {
		wGuiNode *child = wGuiGetChild(node, i);
		wGuiUpdateLayout(child);
	}
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

void wGuiSetSize(wGuiNode *node, wVec2 size)
{
	wAssert(node != NULL);
	node->rect.w = size.x;
	node->rect.h = size.y;
}

wVec2 wGuiGetSize(wGuiNode *node)
{
	wAssert(node != NULL);
	wVec2 ret = { node->rect.w, node->rect.h };
	return ret;
}

/* --------- Image --------- */

typedef struct _wGuiImagePriv
{
	wImage *image;
} wGuiImagePriv;

static void wGuiImage_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wGuiImagePriv *priv = self->priv;
	wPainterDrawImage(painter, wGuiGetGeometry(self), priv->image);
	wGuiNode_paint(self, painter);
}

static void wGuiImage_layout(wGuiNode *self)
{
	wRect rect = wGuiGetGeometry(self);

	for (int i = 0; i < wGuiGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiGetChild(self, i);
		wGuiSetGeometry(child, rect);
	}
}

wGuiNode *wGuiImage()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiImagePriv));
	if (!ret)
		return NULL;

	ret->type = W_GUI_IMAGE;
	ret->paint = wGuiImage_paint;
	ret->layout = wGuiImage_layout;

	return ret;
}

/* --------- Button --------- */

typedef struct _wGuiButtonPriv
{
	bool disabled;
} wGuiButtonPriv;

static void wGuiButton_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wRect rect = wGuiGetGeometry(self);
	wPainterDrawRect(painter, rect);
	wGuiNode_paint(self, painter);
}

static void wGuiButton_layout(wGuiNode *self)
{
	wRect rect = wGuiGetGeometry(self);

	for (int i = 0; i < wGuiGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiGetChild(self, i);
		wGuiSetGeometry(child, rect);
	}
}

static void wGuiButton_mouseEvent(wGuiNode *self)
{
	wAssert(self != NULL);
	wLogInfo("Button clicked");
}

static void wGuiButton_keyboardEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiButton()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiButtonPriv));
	if (!ret)
		return NULL;

	ret->type = W_GUI_BUTTON;
	ret->paint = wGuiButton_paint;
	ret->layout = wGuiButton_layout;
	ret->mouseEvent = wGuiButton_mouseEvent;
	ret->keyboardEvent = wGuiButton_keyboardEvent;

	return ret;
}

/* --------- Label --------- */

typedef struct _wGuiLabelPriv
{
	wString text;
} wGuiLabelPriv;

static void wGuiLabel_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiLabel()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiLabelPriv));
	if (!ret)
		return NULL;

	ret->type = W_GUI_LABEL;
	ret->paint = wGuiLabel_paint;
	return ret;
}

/* --------- Slider --------- */

typedef struct _wGuiSliderPriv
{
	int min;
	int max;
} wGuiSliderPriv;

static void wGuiSlider_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wAssert(painter != NULL);

	wGuiStyle *style = self->style;
	wRect rect = wGuiGetGeometry(self);

	wRect handle = { 0, 0, style->sliderHandleSize, rect.h};
	wRect gutter = { 0, 0, rect.w, style->sliderGutterSize };

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

wGuiNode *wGuiSlider()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiSliderPriv));
	if (!ret)
		return NULL;

	ret->type = W_GUI_SLIDER;
	ret->paint = wGuiSlider_paint;
	ret->mouseEvent = wGuiSlider_mouseEvent;
	ret->keyboardEvent = wGuiSlider_keyboardEvent;

	return ret;
}

/* --------- ScrollArea --------- */

struct _wGuiScrollPriv
{
	bool scrollX;
	bool scrollY;
} wGuiScrollPriv;

static void wGuiScrollArea_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
}

static void wGuiScrollArea_mouseEvent(wGuiNode *self)
{
	wAssert(self != NULL);
}

wGuiNode *wGuiScrollArea()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiScrollPriv));
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
	float hsize = rect.h;

	float minsize = 0.0f;

	for (int i = 0; i < wGuiGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiGetChild(self, i);
		minsize += child->minSize.y;
	}

	float h = (hsize - minsize) / wGuiGetNumChildren(self);
	float y = 0.0f;

	for (int i = 0; i < wGuiGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiGetChild(self, i);
		wRect crect = { 0, y, rect.w, h };
		wGuiSetGeometry(child, crect);
		y += crect.h;
	}
}

wGuiNode *wGuiVBox()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(0);
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

wGuiNode *wGuiHBox()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(0);
	if (!ret)
		return ret;

	ret->type = W_GUI_HBOX;
	ret->layout = wGuiHBox_layout;

	return ret;
}

/* --------- Grid --------- */

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
	if (!ret)
		return NULL;

	ret->type = W_GUI_GRID;
	ret->layout = wGuiGrid_layout;

	return ret;
}

/* --------- Script --------- */

typedef struct _wGuiScriptPriv
{
	wString filename;
} wGuiScriptPriv;

static void wGuiScript_paint(wGuiNode *self, wPainter *painter)
{
}

static void wGuiScript_layout(wGuiNode *self)
{
}

static void wGuiScript_mouseEvent(wGuiNode *self)
{
}

wGuiNode *wGuiScriptNode()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(sizeof(wGuiScriptPriv));
	if (!ret)
		return NULL;

	ret->type = W_GUI_SCRIPT;
	ret->paint = wGuiScript_paint;
	ret->layout = wGuiScript_layout;
	ret->mouseEvent = wGuiScript_mouseEvent;

	return ret;
}
