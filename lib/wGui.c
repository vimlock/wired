#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"

#include <string.h>

const wClass wGuiStyleClass =
{
	.name = "GuiStyle",
	.base = NULL,
	.version = 1,
};

const wClass wGuiNodeClass = 
{
	.name = "GuiNode",
	.base = NULL,
	.version = 1,
};

static wGuiStyle wGuiDefaultStyle =
{
	.class = &wGuiStyleClass,
	.sliderGutterSize = 5.0f,
	.sliderHandleSize = 5.0f,

	.button = {
		.textColor = { 1, 1, 1, 1 },
		.backgroundColor = { 0, 0, 0.2, 1 },
		.borderColor = { 0.8, 0.8, 0.8, 1 },
		.margin = 4.0f
	},

	.buttonHover = {
		.textColor = { 1, 1, 1, 1 },
		.backgroundColor = { 0, 0, 0.1, 1 },
		.borderColor = { 1.0, 1.0, 1.0, 1 },
		.margin = 4.0f
	},

	.buttonPressed = {
		.textColor = { 0.5, 0.5, 0.5, 1 },
		.backgroundColor = { 0, 0, 0.2, 1 },
		.borderColor = { 0.5, 0.5, 0.5, 1 },
		.margin = 4.0f
	},
	.buttonDisabled = {
		.textColor = { 0.5, 0.5, 0.5, 1 },
		.backgroundColor = { 0, 0, 0.2, 1 },
		.borderColor = { 0.5, 0.5, 0.5, 1 },
		.margin = 4.0f
	},

	.label = {
		.textColor = { 1, 1, 1, 1 },
		.margin = 1.0f
	}
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
	ret->enabled = true;
	ret->paint = wGuiNode_paint;
	ret->priv = (ret + 1);
	ret->style = &wGuiDefaultStyle;
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
