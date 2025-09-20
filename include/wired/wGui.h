#pragma once

#include "wMath.h"
#include "wGraphics.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct _wPainter wPainter;
typedef struct _wString wString;
typedef struct _wGuiNode wGuiNode;

enum wGuiNodeType
{
	W_GUI_IMAGE,
	W_GUI_LABEL,
	W_GUI_INPUT,
	W_GUI_SLIDER,
	W_GUI_BUTTON,
	W_GUI_GRID,
	W_GUI_VBOX,
	W_GUI_HBOX,
	W_GUI_SCROLL,
	W_GUI_SCRIPT,
};

struct _wGuiNode
{
	int type;

	wRect rect;
	bool visible;

	wGuiNode *parent;
	wGuiNode **children;
	int numChildren;

	wImage *image;

	void (*layout)(wGuiNode *self);
	void (*paint)(wGuiNode *self, wPainter *painter);
	void (*mouseEvent)(wGuiNode *self);
	void (*keyboardEvent)(wGuiNode *self);
};

wGuiNode *wGuiNodeAlloc();
void wGuiNodeFree(wGuiNode *node);

void wGuiNodePaint(wGuiNode *node, wPainter *painter);

void wGuiDestroy(wGuiNode *node);

void wGuiUpdateLayout(wGuiNode *node);

int wGuiGetNumChildren(wGuiNode *node);
wGuiNode *wGuiGetChild(wGuiNode *node, int child);

void wGuiSetGeometry(wGuiNode *node, wRect rect);
wRect wGuiGetGeometry(wGuiNode *node);

void wGuiSetVisible(wGuiNode *node, bool visible);
bool wGuiIsVisible(wGuiNode *node);

wGuiNode *wGuiImage(wGuiNode *parent);
wGuiNode *wGuiButton(wGuiNode *parent);
wGuiNode *wGuiLabel(wGuiNode *parent);
wGuiNode *wGuiVBox(wGuiNode *parent);
wGuiNode *wGuiHBox(wGuiNode *parent);
wGuiNode *wGuiGrid(wGuiNode *parent);
wGuiNode *wGuiSlider(wGuiNode *parent);
wGuiNode *wGuiScrollArea(wGuiNode *parent);
wGuiNode *wGuiScript(wGuiNode *parent);

/* ---------- Clipboard ---------- */

void wClipboardSetText(const wString *str);
void wClipboardGetText(wString *buf);
void wClipboardGetImage(wImage *img);
