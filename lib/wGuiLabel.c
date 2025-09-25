#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"

static const wClass wGuiLabelClass =
{
	.name = "GuiLabel",
	.base = &wGuiNodeClass,
	.version = 1
};

typedef struct _wGuiLabelPriv
{
	wString *text;
	wColor color;
} wGuiLabelPriv;

static void wGuiLabel_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
	wGuiLabelPriv *priv = self->priv;

	if (!priv->text || wStringSize(priv->text) == 0)
		return;

	wPainterSetColor(painter, priv->color);
	wPainterDrawText(painter, wGuiNodeGetGeometry(self), priv->text);
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

	wGuiLabelPriv *priv = ret->priv;
	priv->text = wStringFromCString("Hello label!");
	priv->color = (wColor){ 0, 0, 0, 1 };

	return ret;
}
