#include "../include/wired/wWrap.h"
#include "../include/wired/wError.h"
#include "../include/wired/wAssert.h"

wGuiNode * wlCheckGuiNode(lua_State *L, int index)
{
	wGuiNode **self = luaL_checkudata(L, index, "GuiNode");
	if (!self)
		return NULL;

	wAssert(*self != NULL);

	return *self;
}

void wlPushGuiNode(lua_State *L, wGuiNode *node)
{
	wGuiNode **self = lua_newuserdata(L, sizeof(wGuiNode*));
	*self = node;

	luaL_getmetatable(L, "GuiNode");
	lua_setmetatable(L, -2);
}

static int wlGuiNodePaint(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	wPainter *painter = wlCheckPainter(L, 2);

	wGuiNodePaint(node, painter);

	return 0;
}

static int wlGuiUpdateLayout(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	wGuiUpdateLayout(node);
	return 0;
}	

static int wlGuiSetSize(lua_State *L)
{
	wGuiNode *self = wlCheckGuiNode(L, 1);
	float w = luaL_checknumber(L, 2);
	float h = luaL_checknumber(L, 3);
	wVec2 size = { w, h };
	wGuiSetSize(self, size);
	return 0;
}

static int wlGuiNodeGetRect(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiNodeAddChild(lua_State *L)
{
	int err;

	wGuiNode *self = wlCheckGuiNode(L, 1);
	wGuiNode *child = wlCheckGuiNode(L, 2);

	err = wGuiNodeAddChild(self, child);
	if (err) {
		luaL_error(L, "%s", wErrorStr(err));
	}

	wlPushGuiNode(L, child);
	return 1;
}

static int wlGuiNodeGetNumChildren(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiGetChild(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	int index = luaL_checkinteger(L, 2);

	wGuiNode *child = wGuiGetChild(node, index);
	if (child)
		wlPushGuiNode(L, child);
	else
		lua_pushnil(L);

	return 1;
}

static int wlGuiGetChildren(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);

	lua_newtable(L);

	for (int i = 0; i < wGuiGetNumChildren(node); ++i) {
		wGuiNode *child = wGuiGetChild(node, i);
		wlPushGuiNode(L, child);
		lua_seti(L, -2, i + 1);
	}

	return 1;
}

static int wlGuiSetVisible(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	bool visible = lua_toboolean(L, 2);
	wGuiSetVisible(node, visible);
	return 0;
}

static int wlGuiImage(lua_State *L)
{
	wGuiNode *node = wGuiImage();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiGrid(lua_State *L)
{
	wGuiNode *node = wGuiGrid();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiLabel(lua_State *L)
{
	wGuiNode *node = wGuiLabel();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiTextArea(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiButton(lua_State *L)
{
	wGuiNode *node = wGuiButton();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiHBox(lua_State *L)
{
	wGuiNode *node = wGuiHBox();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiVBox(lua_State *L)
{
	wGuiNode *node = wGuiVBox();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiScript(lua_State *L)
{
	// TODO
	return 0;
}

static luaL_Reg wlGuiNode[] = {

	{ "SetVisible", wlGuiSetVisible },
	{ "SetSize", wlGuiSetSize },
	{ "Paint",   wlGuiNodePaint },
	{ "GetRect", wlGuiNodeGetRect },
	{ "AddChild", wlGuiNodeAddChild },
	{ "GetNumChildren", wlGuiNodeGetNumChildren },
	{ "GetChild", wlGuiGetChild },
	{ "GetChildren", wlGuiGetChildren },
	{ "UpdateLayout", wlGuiUpdateLayout },

	{ NULL, NULL }
};

static luaL_Reg wlGuiFuncs[] = {
	{ "Image",    wlGuiImage },
	{ "Label",    wlGuiLabel },
	{ "TextArea", wlGuiTextArea },
	{ "Button",   wlGuiButton },
	{ "HBox",     wlGuiHBox },
	{ "VBox",     wlGuiVBox },
	{ "Grid",     wlGuiGrid },
	{ "Script",   wlGuiScript },
};

void wlRegisterGui(lua_State *L)
{
	wlRegisterType(L, "GuiNode", wlGuiNode);

	wlRegisterFunc(L, "GuiButton", wlGuiButton);
	wlRegisterFunc(L, "GuiGrid", wlGuiGrid);
	wlRegisterFunc(L, "GuiHBox", wlGuiHBox);
	wlRegisterFunc(L, "GuiImage", wlGuiImage);
	wlRegisterFunc(L, "GuiLabel", wlGuiLabel);
	wlRegisterFunc(L, "GuiScript", wlGuiScript);
	wlRegisterFunc(L, "GuiVBox", wlGuiVBox);
}
