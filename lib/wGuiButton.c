#include "../include/wired/wGui.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wTexture.h"
#include "../include/wired/wLog.h"

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

	for (int i = 0; i < wGuiNodeGetNumChildren(self); ++i)
		wGuiNodePaint(wGuiNodeGetChild(self, i), painter);

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
