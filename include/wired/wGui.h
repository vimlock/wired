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
	void (*mousePress)(wGuiNode *self, float x, float y,  int button);
	void (*mouseMove)(wGuiNode *self, float x, float y);
	void (*mouseRelease)(wGuiNode *self, float x, float y, int button);
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
void wGuiCanvasMousePress(wGuiNode *node, float x, float y, int button);
void wGuiCanvasMouseMove(wGuiNode *node, float x, float y);
void wGuiCanvasMouseRelease(wGuiNode *node, float x, float y, int button);
wGuiNode *wGuiCanvasPick(wGuiNode *node, float x, float y);

wGuiNode *wGuiImage();
int wGuiImageSetImage(wGuiNode *node, wImage *img);

wGuiNode *wGuiButton();
void wGuiButtonSetImage(wGuiNode *node, wImage *img);

wGuiNode *wGuiLabel();
void wGuiLabelSetText(wGuiNode *node, const wString *str);
void wGuiLabelSetColor(wGuiNode *node, wColor col);

wGuiNode *wGuiVBox();

wGuiNode *wGuiHBox();

wGuiNode *wGuiGrid();
int wGuiGridSetSize(wGuiNode *node, int cols);

wGuiNode *wGuiSlider();
int wGuiSliderSetRange(wGuiNode *node, int min, int max);

wGuiNode *wGuiScrollArea();
wGuiNode *wGuiScript();

extern const wClass wGuiNodeClass;

/* ---------- Clipboard ---------- */

void wClipboardSetText(const wString *str);
void wClipboardGetText(wString *buf);
void wClipboardGetImage(wImage *img);

#ifdef __cplusplus
}
#endif
