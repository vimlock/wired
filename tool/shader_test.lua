
shader = Shader()
shader:SetVertexSource([[
#version 450
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec2 iTexcoord;

uniform mat4 uMvp;

out vec2 vTexcoord;

void main()
{
	gl_Position = uMvp * vec4(iPosition, 1.0);
	vTexcoord = iTexcoord;
}
]])

shader:SetFragmentSource([[
#version 450
in vec2 vTexcoord;

out vec4 fColor;

uniform vec4 uColor = vec4(1, 1, 1, 1);
uniform sampler2D uTexture;

void main()
{
	vec4 texColor = texture(uTexture, vTexcoord);
	fColor = uColor * texColor;
}
]])

shader:Compile()

font = Font()
font:Load("font.ttf")

painter = Painter()
painter:SetShader(shader)
painter:SetFont(font)
painter:SetViewport(0, 0, 800, 600)

canvas = GuiCanvas()
layer = TileLayer(1024, 1024)
layer:SetSheet({Rect(0, 0, 32, 32), Rect(0, 0, 32, 32)})

anim = SpriteAnim()
anim:SetAppendFrame(1, Rect(0, 0, 32, 32))
anim:SetAppendFrame(1, Rect(32, 0, 32, 32))

sheet = SpriteSheet()
sheet:AddState("default", anim)
sheet:AddState("walking", anim)

layer:SetTile(0, 0, 1)
layer:SetTile(1, 0, 1)
layer:SetTile(2, 0, 1)
layer:SetTile(3, 0, 1)
layer:SetTile(3, 5, 1)
layer:LoadRegion(0, 0, 32, 32)

vbox = canvas:AddChild(GuiVBox())

lbl = vbox:AddChild(GuiLabel())
lbl:SetText("Hello")

btn = vbox:AddChild(GuiButton())
btn:SetText("Label")
btn = vbox:AddChild(GuiButton())
btn:SetText("Label")
btn = vbox:AddChild(GuiButton())
btn:SetText("Label")
btn = vbox:AddChild(GuiButton())
btn:SetText("Label")

function update()
	painter:Clear(Color(0.1, 0.1, 0.1, 1))
	painter:SetColor(Color(1, 1, 1, 1))

	layer:Draw(painter)

	-- canvas:Paint(painter)
end

function MousePress(x, y, button)
	canvas:MousePress(x, y, button)
end

function MouseRelease(x, y, button)
	canvas:MouseRelease(x, y, button)
end

function MouseMove(x, y)
	canvas:MouseMove(x, y)
end
