#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

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

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i)
		wGuiNodePaint(wGuiNodeGetChild(self, i), painter);
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
	}

	if (node->clickable && wGuiNodeContainsPoint(node, x, y))
		return node;

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
