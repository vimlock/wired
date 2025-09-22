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

typedef struct _wGuiStyle
{
	int sliderGutterSize;
	int sliderHandleSize;
} wGuiStyle;

struct _wGuiNode
{
	int type;

	wRect rect;

	wIVec2 minSize;
	wIVec2 maxSize;

	bool visible;

	wGuiNode *parent;
	wGuiNode **children;
	int numChildren;

	wGuiStyle *style;

	void *priv;

	void (*layout)(wGuiNode *self);
	void (*paint)(wGuiNode *self, wPainter *painter);
	void (*mouseEvent)(wGuiNode *self);
	void (*keyboardEvent)(wGuiNode *self);
};

wGuiNode *wGuiNodeAlloc(int privSize);
void wGuiNodeFree(wGuiNode *node);

void wGuiNodePaint(wGuiNode *node, wPainter *painter);

void wGuiDestroy(wGuiNode *node);

void wGuiUpdateLayout(wGuiNode *node);

int wGuiGetNumChildren(wGuiNode *node);
wGuiNode *wGuiGetChild(wGuiNode *node, int child);

int wGuiNodeAddChild(wGuiNode *node, wGuiNode *child);

void wGuiSetGeometry(wGuiNode *node, wRect rect);
wRect wGuiGetGeometry(wGuiNode *node);

wVec2 wGuiGetPosition(wGuiNode *node);
void wGuiSetPosition(wGuiNode *node, wVec2 pos);

wVec2 wGuiGetSize(wGuiNode *node);
void wGuiSetSize(wGuiNode *node, wVec2 size);

void wGuiSetVisible(wGuiNode *node, bool visible);
bool wGuiIsVisible(wGuiNode *node);

wGuiNode *wGuiImage();
int wGuiImageSetImage(wGuiNode *node, wImage *img);

wGuiNode *wGuiButton();

wGuiNode *wGuiLabel();
int wGuiLabelSetText(wGuiNode *node, wString *str);

wGuiNode *wGuiVBox();

wGuiNode *wGuiHBox();

wGuiNode *wGuiGrid();
int wGuiGridSetSize(wGuiNode *node, int cols);

wGuiNode *wGuiSlider();
int wGuiSliderSetRange(wGuiNode *node, int min, int max);

wGuiNode *wGuiScrollArea();
wGuiNode *wGuiScript();

/* ---------- Clipboard ---------- */

void wClipboardSetText(const wString *str);
void wClipboardGetText(wString *buf);
void wClipboardGetImage(wImage *img);
