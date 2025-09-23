#pragma once

#include "wMath.h"
#include "wGraphics.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

typedef struct _wPainter wPainter;
typedef struct _wString wString;
typedef struct _wGuiNode wGuiNode;
typedef struct _wClass wClass;

typedef struct _wGuiStyle
{
	int sliderGutterSize;
	int sliderHandleSize;
} wGuiStyle;

struct _wGuiNode
{
	const wClass *class;

	bool visible;
	bool enabled;

	bool hovered;

	wRect rect;

	wIVec2 minSize;
	wIVec2 maxSize;

	wGuiNode *parent;
	wGuiNode **children;
	int numChildren;

	wGuiStyle *style;

	void (*free)(wGuiNode *self);
	void (*layout)(wGuiNode *self);
	void (*paint)(wGuiNode *self, wPainter *painter);
	void (*mouseEvent)(wGuiNode *self);
	void (*keyboardEvent)(wGuiNode *self);

	void *priv;
};

wGuiNode *wGuiNodeAlloc(int privSize);
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
bool wGuiNodeContainsPoint(wGuiNode *node, float x, float y);

wGuiNode *wGuiCanvas();
void wGuiCanvasMousePress(wGuiNode *node, int x, int y);
void wGuiCanvasMouseMove(wGuiNode *node, int x, int y);
void wGuiCanvasMouseRelease(wGuiNode *node, int x, int y);
wGuiNode *wGuiCanvasPick(wGuiNode *node, int x, int y);

wGuiNode *wGuiImage();
int wGuiImageSetImage(wGuiNode *node, wImage *img);

wGuiNode *wGuiButton();
void wGuiButtonSetImage(wGuiNode *node, wImage *img);

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

#ifdef __cplusplus
}
#endif
