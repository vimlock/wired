
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

void main()
{
	fColor = vec4(0, 0, 0, 1);
	fColor.rg = vTexcoord;
}
]])

shader:Compile()

painter = Painter()
painter:SetViewport(0, 0, 800, 600)
painter:SetShader(shader)

vbox = GuiVBox()
vbox:SetSize(800, 600)
btn1 = vbox:AddChild(GuiButton())
btn2 = vbox:AddChild(GuiButton())
btn2 = vbox:AddChild(GuiButton())
btn2 = vbox:AddChild(GuiButton())

vbox:UpdateLayout()

function update()
	painter:Clear(Color(0.1, 0.1, 0.1, 1))
	vbox:Paint(painter)
end
