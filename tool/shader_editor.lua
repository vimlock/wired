TabBar = class()

function TabBar:Init()
	self.layout = Gui.VBox()
	self.tabButtons = Gui.HBox(self.layout)
	self.tabs = Gui.Node(self.layout)
end

function TabBar:AddTab(name, node)
	button = Gui.Button()
	label  = Gui.Label(button)

	self.tabButtons:AddChild(button)
	self.tabs:AddChild(node)
end

function TabBar:SetCurrentIndex(index)
	for tab in self.tabs do
		tab:SetVisible(false)
	end

	self.tabs[index]:SetVisible(true)
end

SpritePreview = class()

function SpritePreview:Init()
	self.shader = nil
	self.image = nil
end

function SpritePreview:Paint(painter)

	local rect = self:GetRect():Shrink(8)

	painter:UseShader(self.shader)
	painter:DrawImage(rect, self.image)
	painter:DrawRect(rect)
end

SpriteEditor = class()

function SpriteEditor:Init()
	self.layout = Gui.HBox()

	tabs = Gui.Script(TabBar, self.layout)

	self.vertEditor = Gui.TextArea()
	self.fragEditor = Gui.TextArea()

	tabs:AddTab(self.vertEditor)
	tabs:AddTab(self.fragEditor)

	preview = Gui.Script(SpritePreview, self.layout)
end
