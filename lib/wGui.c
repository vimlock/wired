#include "../include/wired/wGui.h"
#include "../include/wired/wTexture.h"
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

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i)
		wGuiNodePaint(wGuiNodeGetChild(self, i), painter);
}

wGuiNode *wGuiNodeAlloc()
{
	wGuiNode *ret;

	ret = wMemAlloc(sizeof(wGuiNode));
	memset(ret, 0x0, sizeof(wGuiNode));
	ret->visible = true;
	return ret;
}

void wGuiNodeFree(wGuiNode *node)
{
	if (!node)
		return;

	wGuiWidgetFree(node->widget);
	wMemFree(node);
}

void wGuiNodeAddChild(wGuiNode *node, wGuiNode *child)
{
	node->children = wMemRealloc(node->children, (node->numChildren + 1) * sizeof(wGuiNode*));
	node->children[node->numChildren++] = child;
	child->parent = node;
}

void wGuiNodeRemoveChild(wGuiNode *node, int child)
{
	if (child < 0 || child >= node->numChildren)
		return;

	wGuiNodeFree(node->children[child]);

	for (int i = child; i < node->numChildren - 1; ++i)
		node->children[i] = node->children[i + 1];

	node->numChildren--;
	node->children = wMemRealloc(node->children, node->numChildren * sizeof(wGuiNode*));
}

void wGuiNodePaint(wGuiNode *node, wPainter *painter)
{
	if (!node->visible)
		return;

	if (!node->widget || !node->widget->paint)
		return;

	wPainterPushState(painter);
	// wPainterTranslate(painter, node->rect.x, node->rect.y, 0.0f);
	node->widget->paint(node->widget, painter);
	wPainterPopState(painter);
}

void wGuiNodeUpdateLayout(wGuiNode *node)
{
	if (!node->visible)
		return;

	if (!node->widget || !node->widget->layout)
		return;

	node->widget->layout(node->widget);

	for (int i = 0; i < wGuiNodeGetNumChildren(node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(node, i);
		wGuiNodeUpdateLayout(child);
	}
}

int wGuiNodeGetNumChildren(wGuiNode *node)
{
	return node->numChildren;
}

wGuiNode *wGuiNodeGetChild(wGuiNode *node, int child)
{
	if (child < 0 || child >= node->numChildren)
		return NULL;

	return node->children[child];
}

void wGuiNodeDestroy(wGuiNode *node)
{
	wAssert(node != NULL);
}

wRect wGuiNodeGetGeometry(wGuiNode *node)
{
	wAssert(node != NULL);
	return node->rect;
}

void wGuiNodeSetGeometry(wGuiNode *node, wRect rect)
{
	wAssert(node != NULL);
	node->rect = rect;
}

void wGuiNodeSetVisible(wGuiNode *node, bool visible)
{
	wAssert(node != NULL);
	node->visible = visible;
}

bool wGuiNodeIsVisible(wGuiNode *node)
{
	wAssert(node != NULL);
	return node->visible;
}

void wGuiNodeSetSize(wGuiNode *node, wVec2 size)
{
	wAssert(node != NULL);
	node->rect.w = size.x;
	node->rect.h = size.y;
}

wVec2 wGuiNodeGetSize(wGuiNode *node)
{
	wAssert(node != NULL);
	wVec2 ret = { node->rect.w, node->rect.h };
	return ret;
}

wGuiWidget *wGuiWidgetAlloc(int privSize)
{
	wGuiWidget *ret;

	ret = wMemAlloc(sizeof(wGuiWidget) + privSize);
	memset(ret, 0x0, sizeof(wGuiWidget) + privSize);
	ret->priv = (char*)ret + sizeof(wGuiWidget);

	return ret;
}

void wGuiWidgetFree(wGuiWidget *widget)
{
	if (!widget)
		return;

	if (widget->free)
		widget->free(widget);

	wMemFree(widget);
}

/* --------- Image --------- */

typedef struct _wGuiImagePriv
{
	wGuiWidget base;
	wImage *image;
} wGuiImagePriv;

static void wGuiImage_paint(wGuiWidget *self, wPainter *painter)
{
	wAssert(self != NULL);
	wGuiImagePriv *priv = self->priv;
	wPainterDrawImage(painter, wGuiNodeGetGeometry(self->node), priv->image);
	wGuiNode_paint(self->node, painter);
}

static void wGuiImage_layout(wGuiWidget *self)
{
	wRect rect = wGuiNodeGetGeometry(self->node);

	for (int i = 0; i < wGuiNodeGetNumChildren(self->node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self->node, i);
		wGuiNodeSetGeometry(child, rect);
	}
}

wGuiWidget *wGuiImage()
{
	wGuiWidget *ret = wGuiWidgetAlloc(sizeof(wGuiImagePriv));
	ret->type = W_GUI_IMAGE;
	ret->paint = wGuiImage_paint;
	ret->layout = wGuiImage_layout;

	return ret;
}

/* --------- Button --------- */

typedef struct _wGuiButtonPriv
{
	wGuiWidget base;
	bool disabled;
	wTexture *texture;
	wImage *image;
} wGuiButtonPriv;

static void wGuiButton_paint(wGuiWidget *self, wPainter *painter)
{
	wAssert(self != NULL);

	wGuiButtonPriv *priv = self->priv;

	wRect rect = wGuiNodeGetGeometry(self->node);

	if (priv->image)
		wPainterDrawTexture(painter, rect, priv->texture);
	else
		wPainterDrawRect(painter, rect);
	wGuiNode_paint(self->node, painter);
}

static void wGuiButton_layout(wGuiWidget *self)
{
	wRect rect = wGuiNodeGetGeometry(self->node);

	for (int i = 0; i < wGuiNodeGetNumChildren(self->node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self->node, i);
		wGuiNodeSetGeometry(child, rect);
	}
}

static void wGuiButton_mouseEvent(wGuiWidget *self)
{
	wAssert(self != NULL);
	wLogInfo("Button clicked");
}

static void wGuiButton_keyboardEvent(wGuiWidget *self)
{
	wAssert(self != NULL);
}

wGuiWidget *wGuiButton()
{
	wGuiWidget *ret = wGuiWidgetAlloc(sizeof(wGuiButtonPriv));
	ret->type = W_GUI_BUTTON;
	ret->paint = wGuiButton_paint;
	ret->layout = wGuiButton_layout;
	ret->mouseEvent = wGuiButton_mouseEvent;
	ret->keyboardEvent = wGuiButton_keyboardEvent;

	wGuiButtonPriv *priv = ret->priv;
	priv->texture = wTextureAlloc();

	return ret;
}

void wGuiButtonSetImage(wGuiWidget *widget, wImage *img)
{
	wAssert(widget != NULL);
	wAssert(widget->type == W_GUI_BUTTON);

	wGuiButtonPriv *priv = widget->priv;
	priv->image = img;
	wTextureLoadFromImage(priv->texture, img);
}

/* --------- Label --------- */

typedef struct _wGuiLabelPriv
{
	wGuiWidget base;
	wString text;
} wGuiLabelPriv;

static void wGuiLabel_paint(wGuiWidget *self, wPainter *painter)
{
	wAssert(self != NULL);
}

static void wGuiLabel_free(wGuiWidget *self)
{
	wAssert(self != NULL);
	wGuiLabelPriv *priv = self->priv;
	wStringFree(&priv->text);
}

wGuiWidget *wGuiLabel()
{
	wGuiWidget *ret = wGuiWidgetAlloc(sizeof(wGuiLabelPriv));
	ret->type = W_GUI_LABEL;
	ret->free = wGuiLabel_free;
	ret->paint = wGuiLabel_paint;
	return ret;
}

/* --------- Slider --------- */

typedef struct _wGuiSliderPriv
{
	wGuiWidget base;
	int min;
	int max;
} wGuiSliderPriv;

static void wGuiSlider_free(wGuiWidget *self)
{
	wAssert(self != NULL);
}

static void wGuiSlider_paint(wGuiWidget *self, wPainter *painter)
{
	wAssert(self != NULL);
	wAssert(painter != NULL);

	wGuiStyle *style = self->style;
	wRect rect = wGuiNodeGetGeometry(self->node);

	wRect handle = { 0, 0, style->sliderHandleSize, rect.h};
	wRect gutter = { 0, 0, rect.w, style->sliderGutterSize };

	wPainterDrawRect(painter, gutter);
	wPainterDrawRect(painter, handle);
}

static void wGuiSlider_mouseEvent(wGuiWidget *self)
{
	wAssert(self != NULL);
}

static void wGuiSlider_keyboardEvent(wGuiWidget *self)
{
	wAssert(self != NULL);
}

wGuiWidget *wGuiSlider()
{
	wGuiWidget *ret = wGuiWidgetAlloc(sizeof(wGuiSliderPriv));
	ret->type = W_GUI_SLIDER;
	ret->paint = wGuiSlider_paint;
	ret->free = wGuiSlider_free;
	ret->mouseEvent = wGuiSlider_mouseEvent;
	ret->keyboardEvent = wGuiSlider_keyboardEvent;

	return ret;
}

/* --------- ScrollArea --------- */

struct _wGuiScrollPriv
{
	wGuiWidget base;
	bool scrollX;
	bool scrollY;
} wGuiScrollPriv;

static void wGuiScrollArea_free(wGuiWidget *self)
{
	wAssert(self != NULL);
}

static void wGuiScrollArea_paint(wGuiWidget *self, wPainter *painter)
{
	wAssert(self != NULL);
}

static void wGuiScrollArea_mouseEvent(wGuiWidget *self)
{
	wAssert(self != NULL);
}

wGuiWidget *wGuiScrollArea()
{
	wGuiWidget *ret = wGuiWidgetAlloc(sizeof(wGuiScrollPriv));
	if (!ret)
		return NULL;

	ret->type = W_GUI_SCROLL;
	ret->paint = wGuiScrollArea_paint;
	ret->mouseEvent = wGuiScrollArea_mouseEvent;
	ret->free = wGuiScrollArea_free;

	return ret;
}

/* --------- VBox --------- */

static void wGuiVBox_layout(wGuiWidget *self)
{
	wAssert(self != NULL);

	wRect rect = wGuiNodeGetGeometry(self->node);
	float hsize = rect.h;

	float minsize = 0.0f;

	for (int i = 0; i < wGuiNodeGetNumChildren(self->node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self->node, i);
		minsize += child->minSize.y;
	}

	float h = (hsize - minsize) / wGuiNodeGetNumChildren(self->node);
	float y = 0.0f;

	for (int i = 0; i < wGuiNodeGetNumChildren(self->node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self->node, i);
		wRect crect = { 0, y, rect.w, h };
		wGuiNodeSetGeometry(child, crect);
		y += crect.h;
	}
}

wGuiWidget *wGuiVBox()
{
	wGuiWidget *ret;

	ret = wGuiWidgetAlloc(0);
	ret->type = W_GUI_VBOX;
	ret->layout = wGuiVBox_layout;

	return ret;
}

/* --------- HBox --------- */

static void wGuiHBox_layout(wGuiWidget *self)
{
	wAssert(self != NULL);
}

wGuiWidget *wGuiHBox()
{
	wGuiWidget *ret;

	ret = wGuiWidgetAlloc(0);
	ret->type = W_GUI_HBOX;
	ret->layout = wGuiHBox_layout;

	return ret;
}

/* --------- Grid --------- */

struct _wGuiGridPriv
{
	wGuiWidget base;
	int cols;
} wGuiGridPriv;

static void wGuiGrid_layout(wGuiWidget *self)
{
	wAssert(self != NULL);
}

wGuiWidget *wGuiGrid()
{
	wGuiWidget *ret;

	ret = wGuiWidgetAlloc(sizeof(wGuiGridPriv));
	ret->type = W_GUI_GRID;
	ret->layout = wGuiGrid_layout;

	return ret;
}

/* --------- Script --------- */

typedef struct _wGuiScriptPriv
{
	wGuiWidget base;
	wString filename;
} wGuiScriptPriv;

static void wGuiScript_free(wGuiWidget *self)
{
	wAssert(self != NULL);
	wGuiScriptPriv *priv = self->priv;
	wStringFree(&priv->filename);
}

static void wGuiScript_paint(wGuiWidget *self, wPainter *painter)
{
}

static void wGuiScript_layout(wGuiWidget *self)
{
}

static void wGuiScript_mouseEvent(wGuiWidget *self)
{
}

wGuiWidget *wGuiScript()
{
	wGuiWidget *ret;

	ret = wGuiWidgetAlloc(sizeof(wGuiScriptPriv));
	ret->type = W_GUI_SCRIPT;
	ret->paint = wGuiScript_paint;
	ret->free = wGuiScript_free;
	ret->layout = wGuiScript_layout;
	ret->mouseEvent = wGuiScript_mouseEvent;

	return ret;
}
