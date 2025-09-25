#include "../include/wired/wGui.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wFont.h"

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

	wFont *font = wPainterGetFont(painter);
	wRect textRect = wFontGetRect(font, priv->text);

	wRect rect = wGuiNodeGetGeometry(self);
	float mx = (rect.w - textRect.w) * 0.5f;
	float my = (rect.h - textRect.h) * 0.5f;

	rect.x += mx;
	rect.y += my;
	rect.w -= mx;
	rect.h -= my;

	wPainterSetColor(painter, priv->color);
	wPainterDrawText(painter, rect, priv->text);
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
	priv->color = (wColor){ 1, 1, 1, 1 };

	return ret;
}

void wGuiLabelSetText(wGuiNode *self, const wString *str)
{
	wAssert(self != NULL);
	wAssert(self->class == &wGuiLabelClass);

	wGuiLabelPriv *priv = self->priv;
	wStringAssign(priv->text, str);
}

void wGuiLabelSetColor(wGuiNode *self, wColor col)
{
	wAssert(self != NULL);
	wAssert(self->class == &wGuiLabelClass);

	wGuiLabelPriv *priv = self->priv;
	priv->color = col;
}
