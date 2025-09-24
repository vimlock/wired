
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

painter = Painter()
painter:SetShader(shader)

img = Image()
img:Load("test.png")

canvas = GuiCanvas()

vbox = canvas:AddChild(GuiVBox())

btn1 = vbox:AddChild(GuiButton())
btn2 = vbox:AddChild(GuiButton())
btn3 = vbox:AddChild(GuiButton())
btn4 = vbox:AddChild(GuiButton())

btn1:SetImage(img)
btn2:SetImage(img)
btn3:SetImage(img)
btn4:SetImage(img)

function update()
	painter:Clear(Color(0.1, 0.1, 0.1, 1))
	painter:SetColor(Color(1, 1, 1, 1))
	canvas:Paint(painter)
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
