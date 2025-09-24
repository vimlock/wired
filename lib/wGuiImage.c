#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wPainter.h"

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

