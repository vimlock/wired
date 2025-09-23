#include "../include/wired/wWrap.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wAssert.h"

void *wlCheckClass(lua_State *L, int index, const char *name)
{
	wAssert(L != NULL);

	if (!lua_isuserdata(L, index))
		luaL_error(L, "Expecting %s", name);

	lua_getmetatable(L, index);
	while (lua_istable(L, -1)) {
		luaL_getmetatable(L, name);
		if (lua_rawequal(L, -1, -1)) {
			lua_pop(L, 2);
			return *(void**)lua_touserdata(L, index);
		}

		lua_pop(L, 1);
		lua_getfield(L, -1, "__index");
		if (!lua_istable(L, -1)) {
			lua_pop(L, 2);
			luaL_error(L, "Type mismatch");
		}

		lua_remove(L, -2);
	}

	luaL_error(L, "Type mismatch");
	return NULL;
}

void wlPushClass(lua_State *L, void *obj, const char *cls)
{
	wAssert(L != NULL);
	wAssert(cls != NULL);
	wAssert(obj != NULL);

	int res;

	void **self = lua_newuserdata(L, sizeof(void*));
	*self = obj;
	res = luaL_getmetatable(L, cls);
	if (!res) {
		luaL_error(L, "Metatable for %s not registered", cls);
	}

	lua_setmetatable(L, -2);
}

wGuiNode * wlCheckGuiNode(lua_State *L, int index)
{
	return wlCheckClass(L, index, "GuiNode");
}

void wlPushGuiNode(lua_State *L, wGuiNode *node)
{
	wlPushClass(L, node, node->class->name);
}

static int wlGuiNodePaint(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	wPainter *painter = wlCheckPainter(L, 2);

	wGuiNodePaint(node, painter);

	return 0;
}

static int wlGuiNodeUpdateLayout(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	wGuiNodeUpdateLayout(node);
	return 0;
}	

static int wlGuiNodeSetSize(lua_State *L)
{
	wGuiNode *self = wlCheckGuiNode(L, 1);
	float w = luaL_checknumber(L, 2);
	float h = luaL_checknumber(L, 3);
	wVec2 size = { w, h };
	wGuiNodeSetSize(self, size);
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

	wGuiNodeAddChild(self, child);
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

	wGuiNode *child = wGuiNodeGetChild(node, index);
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

	for (int i = 0; i < wGuiNodeGetNumChildren(node); ++i) {
		wGuiNode *child = wGuiNodeGetChild(node, i);
		wlPushGuiNode(L, child);
		lua_seti(L, -2, i + 1);
	}

	return 1;
}

static int wlGuiNodeSetVisible(lua_State *L)
{
	wGuiNode *node = wlCheckGuiNode(L, 1);
	bool visible = lua_toboolean(L, 2);
	wGuiNodeSetVisible(node, visible);
	return 0;
}

wGuiNode *wlCheckGuiButton(lua_State *L, int index)
{
	return wlCheckClass(L, index, "GuiButton");
}

void wlPushGuiButton(lua_State *L, wGuiNode *node)
{
	wlPushClass(L, node, "GuiButton");
}

static int wlGuiButton__new(lua_State *L)
{
	wGuiNode *node = wGuiButton();
	wlPushGuiButton(L, node);
	return 1;
}

static int wlGuiButtonSetImage(lua_State *L)
{
	wGuiNode *node = wlCheckGuiButton(L, 1);
	wImage *img = wlCheckImage(L, 2);
	wGuiButtonSetImage(node, img);
	return 0;
}	

static int wlGuiButtonGetImage(lua_State *L)
{
	wGuiNode *node = wlCheckGuiButton(L, 1);
	return 1;
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

wGuiNode *wlCheckGuiCanvas(lua_State *L, int index)
{
	return wlCheckClass(L, index, "GuiCanvas");
}

void wlPushGuiCanvas(lua_State *L, wGuiNode *node)
{
	wlPushClass(L, node, "GuiCanvas");
}

static int wlGuiCanvas__new(lua_State *L)
{
	wGuiNode *node = wGuiCanvas();
	wlPushGuiCanvas(L, node);
	return 1;
}

static int wlGuiCanvasMousePress(lua_State *L)
{
	wGuiNode *self = wlCheckGuiCanvas(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	wGuiCanvasMousePress(self, x, y);
	return 0;
}

static int wlGuiCanvasMouseRelease(lua_State *L)
{
	wGuiNode *self = wlCheckGuiCanvas(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	wGuiCanvasMouseRelease(self, x, y);
	return 0;
}

static int wlGuiCanvasMouseMove(lua_State *L)
{
	wGuiNode *self = wlCheckGuiCanvas(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	wGuiCanvasMouseMove(self, x, y);
	return 0;
}

static luaL_Reg wlGuiNode[] = {

	{ "SetVisible", wlGuiNodeSetVisible },
	{ "SetSize", wlGuiNodeSetSize },
	{ "Paint",   wlGuiNodePaint },
	{ "GetRect", wlGuiNodeGetRect },
	{ "AddChild", wlGuiNodeAddChild },
	{ "GetNumChildren", wlGuiNodeGetNumChildren },
	{ "GetChild", wlGuiGetChild },
	{ "GetChildren", wlGuiGetChildren },
	{ "UpdateLayout", wlGuiNodeUpdateLayout },
	{ NULL, NULL }
};

static luaL_Reg wlGuiButton[] = {
	{ "__new",    wlGuiButton__new },
	{ "SetImage", wlGuiButtonSetImage },
	{ "GetImage", wlGuiButtonGetImage },
	{ NULL, NULL }
};

static luaL_Reg wlGuiImage[] = {
	{ "__new", wlGuiImage__new },
	{ NULL, NULL }
};

static luaL_Reg wlGuiLabel[] = {
	{ "__new", wlGuiLabel__new },
	{ NULL, NULL }
};

static luaL_Reg wlGuiGrid[] = {
	{ "__new", wlGuiGrid__new },
	{ NULL, NULL }
};

static luaL_Reg wlGuiVBox[] = {
	{ "__new", wlGuiVBox__new },
	{ NULL, NULL }
};

static luaL_Reg wlGuiHBox[] = {
	{ "__new", wlGuiHBox__new },
	{ NULL, NULL }
};

static luaL_Reg wlGuiCanvas[] = {
	{ "__new", wlGuiCanvas__new },
	{ "MousePress",   wlGuiCanvasMousePress },
	{ "MouseMove",    wlGuiCanvasMouseMove },
	{ "MouseRelease", wlGuiCanvasMouseRelease },
	{ NULL, NULL }
};

void wlRegisterGui(lua_State *L)
{
	wlRegisterType(L, "GuiNode", wlGuiNode);
	wlRegisterDerivedType(L, "GuiVBox",   "GuiNode", wlGuiVBox);
	wlRegisterDerivedType(L, "GuiButton", "GuiNode", wlGuiButton);
	wlRegisterDerivedType(L, "GuiCanvas", "GuiNode", wlGuiCanvas);
	wlRegisterDerivedType(L, "GuiLabel",  "GuiNode", wlGuiLabel);
}
