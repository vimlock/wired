
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
painter:SetViewport(0, 0, 800, 600)
painter:SetShader(shader)

img = Image()
img:Load("../../../../test.png")

tex = Texture()
tex:LoadFromImage(img)

vbox = GuiVBox()
vbox:SetSize(800, 600)
btn1 = vbox:AddChild(GuiButton({
	image=img
}))
btn2 = vbox:AddChild(GuiButton({
	image=img
}))
btn3 = vbox:AddChild(GuiButton({
	image=img
}))
btn4 = vbox:AddChild(GuiButton({
	image=img
}))

vbox:UpdateLayout()

function update()
	painter:Clear(Color(0.1, 0.1, 0.1, 1))
	painter:SetColor(Color(1, 1, 1, 1))
	-- vbox:Paint(painter)
	painter:DrawTexture(Rect(0, 0,1344/2, 896/2), tex)
end
