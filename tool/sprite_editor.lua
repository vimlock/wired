
HANDLE_EDGE_RIGHT =  1
HANDLE_EDGE_LEFT = 2
HANDLE_EDGE_TOP = 3
HANDLE_EDGE_BOTTOM = 4

HANDLE_CORNER_TL = 5
HANDLE_CORNER_TR = 6
HANDLE_CORNER_BL = 7
HANDLE_CORNER_BR = 8

HANDLE_CENTER = 9

RectEditor = class()

function RectEditor:Init()
	self.rects = {}
	self.selected = nil
	self.handle = nil
end

function RectEditor:Paint(painter)
	painter:DrawImage()

	handleRect = Rect(-4, -4, 8, 8)

	for idx, rect in ipairs(self.sprite.frames) do
		painter:DrawRect(rect)

		if idx == self.selected then
			painter:DrawRect(handleRect:Translated(rect.x, rect.y))
			painter:DrawRect(handleRect:Translated(rect.x + rect.w, rect.y))
			painter:DrawRect(handleRect:Translated(rect.x + rect.w, rect.y + rect.h))
			painter:DrawRect(handleRect:Translated(rect.x, rect.y + rect.h))

			painter:DrawRect(handleRect:Translated(rect.x + rect.w / 2, rect.y))
			painter:DrawRect(handleRect:Translated(rect.x + rect.w / 2, rect.y + rect.h))
			painter:DrawRect(handleRect:Translated(rect.x, rect.y + rect.h / 2))
			painter:DrawRect(handleRect:Translated(rect.x + rect.w, rect.y + rect.h / 2))
		end
	end
end

function RectEditor:AddFrame(before)
end

function RectEditor:RemoveFrame(index)
end

function RectEditor:SetRects(rects)
	self.rects = rects
end

function RectEditor:GetRects()
	return self.rects
end

function RectEditor:MouseEvent()
end


SpriteAnimEditor = class()

function SpriteAnimEditor:Init()

	layout = Gui.HBox()

	sidebar = Gui.VBox(layout)

	self.rectEditor = Gui.Script(RectEditor, layout)
	self.actionSlider = Gui.Slider(sidebar)
	self.recoverySlider = Gui.Slider(sidebar)
end

function SpriteAnimEditor:MouseEvent()
end

function CreateUI()
end

Log.Info("Hello from lua!");
