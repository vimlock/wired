#include "../include/wired/wGui.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wFont.h"
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
	wTexture *texture;
	wImage *image;
	wString *text;
	bool isPressed;

} wGuiButtonPriv;

static void wGuiButton_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);

	wGuiButtonPriv *priv = self->priv;
	wGuiButtonStyle *style = &self->style->button;

	if (self->hovered)
		style = &self->style->buttonHover;

	if (priv->isPressed)
		style = &self->style->buttonPressed;

	if (!self->enabled)
		style = &self->style->buttonDisabled;

	wRect rect = wGuiNodeGetGeometry(self);
	rect = wRectShrink(rect, style->margin);

	wPainterPushState(painter);

	if (priv->image) {
		wPainterSetColor(painter, style->backgroundColor);
		wPainterDrawTexture(painter, rect, priv->texture);
	}
	else {
		wPainterSetColor(painter, style->backgroundColor);
		wPainterDrawSlicedRect(painter, rect);
	}

	if (style->borderColor.a > 0) {
		wPainterSetColor(painter, style->borderColor);
		wPainterDrawBorderRect(painter, rect);
	}

	if (priv->text && wStringSize(priv->text) != 0) {
		wFont *font = wPainterGetFont(painter);
		wRect tmp = rect;
		wRect textRect = wFontGetRect(font, priv->text);

		float mx = (rect.w - textRect.w) * 0.5f;
		float my = (rect.h - textRect.h) * 0.5f;

		tmp.x += mx;
		tmp.y += my;
		tmp.w -= mx;
		tmp.h -= my;

		wPainterSetColor(painter, style->textColor);
		wPainterDrawText(painter, tmp, priv->text);
	}

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
	wGuiButtonPriv *priv = self->priv;
	priv->isPressed = true;
}

static void wGuiButton_mouseRelease(wGuiNode *self, float x, float y, int button)
{
	wAssert(self != NULL);
	wLogInfo("Button released");
	wGuiButtonPriv *priv = self->priv;
	priv->isPressed = false;
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
	ret->clickable = true;
	ret->layout = wGuiButton_layout;
	ret->mousePress = wGuiButton_mousePress;
	ret->mouseRelease = wGuiButton_mouseRelease;
	ret->keyboardEvent = wGuiButton_keyboardEvent;

	wGuiButtonPriv *priv = ret->priv;
	priv->texture = wTextureAlloc();
	priv->text = wStringFromCString("");

	return ret;
}

void wGuiButtonSetImage(wGuiNode *node, wImage *img)
{
	wAssert(node != NULL);

	wGuiButtonPriv *priv = node->priv;
	priv->image = img;
	wTextureLoadFromImage(priv->texture, img);
}

void wGuiButtonSetText(wGuiNode *self, const wString *text)
{
	wAssert(self != NULL);
	wAssert(text != NULL);
	wGuiButtonPriv *priv = self->priv;
	wStringAssign(priv->text, text);
}
