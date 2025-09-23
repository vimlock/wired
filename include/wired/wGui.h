#pragma once

#include "wMath.h"
#include "wGraphics.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct _wPainter wPainter;
typedef struct _wString wString;
typedef struct _wGuiNode wGuiNode;
typedef struct _wGuiWidget wGuiWidget;

enum wGuiWidgetType
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

struct _wGuiWidget
{
	void (*free)(wGuiWidget *self);
	void (*layout)(wGuiWidget *self);
	void (*paint)(wGuiWidget *self, wPainter *painter);
	void (*mouseEvent)(wGuiWidget *self);
	void (*keyboardEvent)(wGuiWidget *self);

	int type;

	wGuiStyle *style;
	wGuiNode *node;
	void *priv;
};

struct _wGuiNode
{
	wRect rect;

	wIVec2 minSize;
	wIVec2 maxSize;

	bool visible;
	bool enabled;
	wGuiWidget *widget;

	wGuiNode *parent;
	wGuiNode **children;
	int numChildren;
};

wGuiNode *wGuiNodeAlloc();
void wGuiNodeFree(wGuiNode *node);
void wGuiNodeUpdateLayout(wGuiNode *node);
void wGuiNodePaint(wGuiNode *node, wPainter *painter);
int wGuiNodeGetNumChildren(wGuiNode *node);
wGuiNode *wGuiNodeGetChild(wGuiNode *node, int child);
void wGuiNodeAddChild(wGuiNode *node, wGuiNode *child);
void wGuiNodeRemoveChild(wGuiNode *node, int child);
wVec2 wGuiNodeGetPosition(wGuiNode *node);
void wGuiNodeSetPosition(wGuiNode *node, wVec2 pos);
wVec2 wGuiNodeGetSize(wGuiNode *node);
void wGuiNodeSetSize(wGuiNode *node, wVec2 size);
void wGuiNodeSetGeometry(wGuiNode *node, wRect rect);
wRect wGuiNodeGetGeometry(wGuiNode *node);
void wGuiNodeSetVisible(wGuiNode *node, bool visible);
bool wGuiNodeIsVisible(wGuiNode *node);

void wGuiNodeSetWidget(wGuiNode *node, wGuiWidget *widget);
wGuiWidget *wGuiNodeGetWidget(wGuiNode *node);
wGuiWidget *wGuiNodeTakeWidget(wGuiNode *node);

wGuiWidget *wGuiWidgetAlloc(int privSize);
void wGuiWidgetFree(wGuiWidget *widget);

wGuiWidget *wGuiImage();
int wGuiImageSetImage(wGuiWidget *node, wImage *img);

wGuiWidget *wGuiButton();
void wGuiButtonSetImage(wGuiWidget *node, wImage *img);

wGuiWidget *wGuiLabel();
int wGuiLabelSetText(wGuiWidget *node, wString *str);

wGuiWidget *wGuiVBox();

wGuiWidget *wGuiHBox();

wGuiWidget *wGuiGrid();
int wGuiGridSetSize(wGuiWidget *node, int cols);

wGuiWidget *wGuiSlider();
int wGuiSliderSetRange(wGuiWidget *node, int min, int max);

wGuiWidget *wGuiScrollArea();
wGuiWidget *wGuiScript();

/* ---------- Clipboard ---------- */

void wClipboardSetText(const wString *str);
void wClipboardGetText(wString *buf);
void wClipboardGetImage(wImage *img);
