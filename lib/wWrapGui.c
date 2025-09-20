#include "../include/wired/wWrap.h"

wGuiNode * wlCheckGuiNode(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "GuiNode");
}

void wlPushGuiNode(lua_State *L, wGuiNode *node)
{
	// TODO
}

static int wlGuiNodePaint(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	wPainter *painter = wlCheckPainter(L, 2);

	wGuiNodePaint(node, painter);

	return 0;
}

static int wlGuiNodeGetRect(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiNodeAddChild(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiNodeGetNumChildren(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiGetChild(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiGetChildren(lua_State *L)
{
	return 0;
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
	wGuiNode *parent = wlCheckGuiNode(L, 1);
	wGuiNode *child = wGuiImage(parent);
	lua_pushlightuserdata(L, child);
	return 1;
}

static int wlGuiGrid(lua_State *L)
{
	wGuiNode *parent = wlCheckGuiNode(L, 1);
	wGuiNode *child = wGuiGrid(parent);
	lua_pushlightuserdata(L, child);
	return 1;
}

static int wlGuiLabel(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiTextArea(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiButton(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiHBox(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiVBox(lua_State *L)
{
	// TODO
	return 0;
}

static int wlGuiScript(lua_State *L)
{
	// TODO
	return 0;
}

static luaL_Reg wlGuiNode[] = {

	{ "SetVisible", wlGuiSetVisible },
	{ "Paint",   wlGuiNodePaint },
	{ "GetRect", wlGuiNodeGetRect },
	{ "AddChild", wlGuiNodeAddChild },
	{ "GetNumChildren", wlGuiNodeGetNumChildren },
	{ "GetChild", wlGuiGetChild },

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
}
