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
} wGuiLabelPriv;

static void wGuiLabel_paint(wGuiNode *self, wPainter *painter)
{
	wAssert(self != NULL);
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
	return ret;
}
