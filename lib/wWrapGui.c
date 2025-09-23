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

static int wlGuiImage__new(lua_State *L)
{
	wGuiNode *node = wGuiImage();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiGrid__new(lua_State *L)
{
	wGuiNode *node = wGuiGrid();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiLabel__new(lua_State *L)
{
	wGuiNode *node = wGuiLabel();
	wlPushGuiNode(L, node);
	return 1;
}

void wlPushGuiButton(lua_State *L, wGuiNode *node)
{
	wGuiNode **self = lua_newuserdata(L, sizeof(wGuiNode*));
	*self = node;

	luaL_getmetatable(L, "GuiButton");
	lua_setmetatable(L, -2);
}

static int wlGuiButton__new(lua_State *L)
{
	wGuiNode *node = wGuiButton();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiButtonSetImage(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	wImage *img = wlCheckImage(L, 2);
	wGuiButtonSetImage(node, img);
	return 0;
}	

static int wlGuiHBox__new(lua_State *L)
{
	wGuiNode *node = wGuiHBox();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiVBox__new(lua_State *L)
{
	wGuiNode *node = wGuiVBox();
	wlPushGuiNode(L, node);
	return 1;
}

static int wlGuiScript__new(lua_State *L)
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

static luaL_Reg wlGuiButton[] = {
	{ "SetImage", wlGuiButtonSetImage },
	{ NULL, NULL }
};

void wlRegisterGui(lua_State *L)
{
	wlRegisterType(L, "GuiNode", wlGuiNode);

	wlRegisterFunc(L, "GuiButton", wlGuiButton__new);
	wlRegisterFunc(L, "GuiGrid",   wlGuiGrid__new);
	wlRegisterFunc(L, "GuiHBox",   wlGuiHBox__new);
	wlRegisterFunc(L, "GuiImage",  wlGuiImage__new);
	wlRegisterFunc(L, "GuiLabel",  wlGuiLabel__new);
	wlRegisterFunc(L, "GuiScript", wlGuiScript__new);
	wlRegisterFunc(L, "GuiVBox",   wlGuiVBox__new);
}
