#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wTexture.h"
#include "../include/wired/wGraphics.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"

#include <string.h>

static const wClass wGuiNodeClass = 
{
	.name = "GuiNode",
	.base = NULL,
	.version = 1,
};

static void wGuiNode_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i)
		wGuiNodePaint(wGuiNodeGetChild(self, i), painter);
}

wGuiNode *wGuiNodeAlloc(int privSize)
{
	wGuiNode *ret;

	ret = wMemAlloc(sizeof(wGuiNode) + privSize);
	memset(ret, 0x0, sizeof(wGuiNode) + privSize);
	ret->class = &wGuiNodeClass;
	ret->numChildren = 0;
	ret->visible = true;
	ret->paint = wGuiNode_paint;
	ret->priv = (ret + 1);
	return ret;
}

void wGuiNodeFree(wGuiNode *node)
{
	if (!node)
		return;

	if (node->free)
		node->free(node);

	wMemFree(node->children);
	node->numChildren = 0;

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

	if (!node->paint)
		return;

	node->paint(node, painter);
}

void wGuiNodeUpdateLayout(wGuiNode *node)
{
	if (!node->visible)
		return;

	if (node->layout)
		node->layout(node);

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

bool wGuiNodeContainsPoint(wGuiNode *node, float x, float y)
{
	wAssert(node != NULL);
	return wRectContains(node->rect, x, y);
}

/* --------- Canvas --------- */

static const wClass wGuiCanvasClass = {
	.name = "GuiCanvas",
	.base = &wGuiNodeClass,
	.version = 1,
};

typedef struct _wGuiCanvasPriv
{
	wGuiNode *hover;
	wGuiNode *press;
} wGuiCanvasPriv;

static void wGuiCanvas_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wAssert(painter != NULL);

	wGuiNodeUpdateLayout(self);

	wRectI viewport = { 0, 0, self->rect.w, self->rect.h };
	wPainterSetViewport(painter, viewport);

	wGuiNode_paint(self, painter);
}

static void wGuiCanvas_layout(wGuiNode *self)
{
	wAssert(self != NULL);

	wVec2 size = { wWindowGetWidth(), wWindowGetHeight() };
	wGuiNodeSetSize(self, size);

	wRect rect = wGuiNodeGetGeometry(self);
	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self, i);
		wGuiNodeSetGeometry(child, rect);
	}
}

wGuiNode *wGuiCanvas()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiCanvasPriv));
	ret->class = &wGuiCanvasClass;
	ret->paint = wGuiCanvas_paint;
	ret->layout = wGuiCanvas_layout;

	return ret;
}

static wGuiNode * pickRecurse(wGuiNode *node, int x, int y)
{
	for (int i = 0; i < wGuiNodeGetNumChildren(node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(node, i);

		wGuiNode *nested = pickRecurse(child, x, y);
		if (nested)
			return nested;

		if (wGuiNodeContainsPoint(child, x, y))
			return child;
	}

	return NULL;
}

wGuiNode *wGuiCanvasPick(wGuiNode *node, float x, float y)
{
	for (int i = 0; i < wGuiNodeGetNumChildren(node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(node, i);
		wGuiNode *nested = pickRecurse(child, x, y);
		if (nested)
			return nested;
	}

	return NULL;
}

void wGuiCanvasMousePress(wGuiNode *node, float x, float y, int button)
{
	wGuiCanvasPriv *priv = node->priv;

	wGuiNode *target = wGuiCanvasPick(node, x, y);
	if (target == priv->press)
		return;

	if (priv->press && priv->press->mouseRelease) {
		priv->press->mouseRelease(priv->press, x, y, button);
	}

	priv->press = target;

	if (priv->press && priv->press->mousePress) {
		priv->press->mousePress(priv->press, x, y, button);
	}
}

void wGuiCanvasMouseMove(wGuiNode *node, float x, float y)
{
	wGuiCanvasPriv *priv = node->priv;

	if (priv->press && priv->press->mouseMove) {
		priv->press->mouseMove(priv->press, x, y);
	}

	if (priv->hover) {
		priv->hover->hovered = false;
		priv->hover = NULL;
	}

	wGuiNode *target = wGuiCanvasPick(node, x, y);
	if (target) {
		priv->hover = target;
		priv->hover->hovered = true;
	}
}

void wGuiCanvasMouseRelease(wGuiNode *node, float x, float y, int button)
{
	wGuiCanvasPriv *priv = node->priv;

	if (priv->press && priv->press->mouseRelease) {
		priv->press->mouseRelease(priv->press, x, y, button);
	}

	priv->press = NULL;
}

/* --------- Image --------- */

static const wClass wGuiImageClass =
{
	.name = "GuiImage",
	.base = &wGuiNodeClass,
	.version = 1
};

typedef struct _wGuiImagePriv
{
	wImage *image;
} wGuiImagePriv;

static void wGuiImage_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wGuiImagePriv *priv = self->priv;
	wPainterDrawImage(painter, wGuiNodeGetGeometry(self), priv->image);
	wGuiNode_paint(self, painter);
}

static void wGuiImage_layout(wGuiNode *self)
{
	wRect rect = wGuiNodeGetGeometry(self);

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self, i);
		wGuiNodeSetGeometry(child, rect);
	}
}

wGuiNode *wGuiImage()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiImagePriv));
	ret->class = &wGuiImageClass;
	ret->paint = wGuiImage_paint;
	ret->layout = wGuiImage_layout;

	return ret;
}

/* --------- Button --------- */

static const wClass wGuiButtonClass =
{
	.name = "GuiButton",
	.base = &wGuiNodeClass,
	.version = 1
};

typedef struct _wGuiButtonPriv
{
	bool disabled;
	wTexture *texture;
	wImage *image;

	wColor tintNormal;
	wColor tintHover;

} wGuiButtonPriv;

static void wGuiButton_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);

	wPainterPushState(painter);

	wGuiButtonPriv *priv = self->priv;

	wRect rect = wGuiNodeGetGeometry(self);

	if (self->hovered)
		wPainterSetColor(painter, priv->tintHover);
	else
		wPainterSetColor(painter, priv->tintNormal);

	if (priv->image)
		wPainterDrawTexture(painter, rect, priv->texture);
	else
		wPainterDrawRect(painter, rect);

	wGuiNode_paint(self, painter);

	wPainterPopState(painter);
}

static void wGuiButton_layout(wGuiNode *self)
{
	wRect rect = wGuiNodeGetGeometry(self);

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i) {
		wGuiNode *child = wGuiNodeGetChild(self, i);
		wGuiNodeSetGeometry(child, rect);
	}
}

static void wGuiButton_mousePress(wGuiNode *self, float x, float y, int button)
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
	ret->class = &wGuiButtonClass;
	ret->paint = wGuiButton_paint;
	ret->layout = wGuiButton_layout;
	ret->mousePress = wGuiButton_mousePress;
	ret->keyboardEvent = wGuiButton_keyboardEvent;

	wGuiButtonPriv *priv = ret->priv;
	priv->texture = wTextureAlloc();
	priv->tintNormal = (wColor){1.0f, 1.0f, 1.0f, 1.0f};
	priv->tintHover  = (wColor){0.8f, 0.8f, 0.8f, 1.0f};

	return ret;
}

void wGuiButtonSetImage(wGuiNode *node, wImage *img)
{
	wAssert(node != NULL);

	wGuiButtonPriv *priv = node->priv;
	priv->image = img;
	wTextureLoadFromImage(priv->texture, img);
}

/* --------- Label --------- */

static const wClass wGuiLabelClass =
{
	.name = "GuiLabel",
	.base = &wGuiNodeClass,
	.version = 1
};

typedef struct _wGuiLabelPriv
{
	wString *text;
} wGuiLabelPriv;

static void wGuiLabel_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
}

static void wGuiLabel_free(wGuiNode *self)
{
	wAssert(self != NULL);
	wGuiLabelPriv *priv = self->priv;
	wStringFree(priv->text);
}

wGuiNode *wGuiLabel()
{
	wGuiNode *ret = wGuiNodeAlloc(sizeof(wGuiLabelPriv));
	ret->class = &wGuiLabelClass;
	ret->free = wGuiLabel_free;
	ret->paint = wGuiLabel_paint;
	return ret;
}

/* --------- Slider --------- */

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

/* --------- ScrollArea --------- */

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

/* --------- VBox --------- */

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

/* --------- HBox --------- */

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

/* --------- Grid --------- */

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

/* --------- Script --------- */

static const wClass wGuiScriptClass =
{
	.name = "GuiScript",
	.base = &wGuiNodeClass,
	.version = 1
};

typedef struct _wGuiScriptPriv
{
	wString *filename;
} wGuiScriptPriv;

static void wGuiScript_free(wGuiNode *self)
{
	wAssert(self != NULL);
	wGuiScriptPriv *priv = self->priv;
	wStringFree(priv->filename);
}

static void wGuiScript_paint(wGuiNode *self, wPainter *painter)
{
}

static void wGuiScript_layout(wGuiNode *self)
{
}

static void wGuiScript_mouseMove(wGuiNode *self, float x, float y)
{
}

wGuiNode *wGuiScript()
{
	wGuiNode *ret;

	ret = wGuiNodeAlloc(sizeof(wGuiScriptPriv));
	ret->class = &wGuiScriptClass;
	ret->paint = wGuiScript_paint;
	ret->free = wGuiScript_free;
	ret->layout = wGuiScript_layout;
	ret->mouseMove = wGuiScript_mouseMove;

	return ret;
}
