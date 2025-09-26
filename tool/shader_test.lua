
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
uniform sampler2DRect uTexture;

void outline(in vec2 texcoord, in vec4 col, out vec4 res)
{
	float alpha = 0.0;
	vec2 offset = 1.0 / textureSize(uTexture, 0);
	alpha += texture(uTexture, texcoord + vec2(-offset.x, 0)).a >= 0.5 ? 1.0 : 0.0;
	alpha += texture(uTexture, texcoord + vec2(+offset.x, 0)).a >= 0.5 ? 1.0 : 0.0;
	alpha += texture(uTexture, texcoord + vec2(0, -offset.y)).a >= 0.5 ? 1.0 : 0.0;
	alpha += texture(uTexture, texcoord + vec2(0, +offset.y)).a >= 0.5 ? 1.0 : 0.0;

	if (alpha > 3.0)
		res = col;
	else if (alpha > 1.0)
		res = vec4(0, 1, 1, 1);
	else
		res = col;
}

void desaturate(in vec4 col, out vec4 res)
{
	vec3 lum = vec3(0.299, 0.587, 0.114);
	vec3 gray = vec3(dot(lum, col.rgb));
	res = vec4(gray, col.a);
}

void main()
{
	vec4 texColor = texture(uTexture, vTexcoord);
	fColor = uColor * texColor;
	// desaturate(fColor, fColor);
	// outline(vTexcoord, fColor, fColor);
}
]])

shader:Compile()

font = Font()
font:Load("font.ttf")

painter = Painter()
painter:SetShader(shader)
painter:SetFont(font)
painter:SetViewport(0, 0, 1600, 1200)

canvas = GuiCanvas()
layer = TileLayer(1024, 1024)
layer:SetSheet({
	Rect(  0, 0, 32, 32),
	Rect( 32, 0, 32, 32),
	Rect( 64, 0, 32, 32),
	Rect(128, 0, 32, 32),
	Rect(0, 128, 32, 32),
	Rect(0, 160, 32, 32),
})

LoadTexture("tex_skeleton", "../skeleton.png")
LoadTexture("tex_demon",    "../Demon.png")
LoadTexture("tex_zealot",   "../zealot.png")
LoadTexture("tex_tileset_grass", "../TX Tileset Grass.png")

anim = SpriteAnim()
-- anim:AppendFrame(2, Rect(30, 10, 80, 100))
-- anim:AppendFrame(2, Rect(140, 10, 80, 100))
-- anim:AppendFrame(2, Rect(230, 10, 80, 100))
-- anim:AppendFrame(2, Rect(350, 10, 80, 100))
-- anim:AppendFrame(2, Rect(470, 10, 80, 100))
-- anim:AppendFrame(2, Rect(570, 10, 80, 100))
-- anim:AppendFrame(2, Rect(670, 10, 80, 100))
-- anim:AppendFrame(2, Rect(760, 10, 80, 100))
-- anim:AppendFrame(2, Rect(670, 10, 80, 100))
-- anim:AppendFrame(2, Rect(570, 10, 80, 100))
-- anim:AppendFrame(2, Rect(470, 10, 80, 100))
anim:AppendFrame(5, Rect(10, 136, 100, 100))
anim:AppendFrame(5, Rect(110, 136, 100, 100))
anim:AppendFrame(5, Rect(210, 136, 100, 100))
anim:AppendFrame(5, Rect(320, 136, 100, 100))
anim:AppendFrame(5, Rect(440, 136, 100, 100))
anim:AppendFrame(5, Rect(540, 136, 100, 100))
anim:AppendFrame(5, Rect(630, 136, 100, 100))
anim:AppendFrame(5, Rect(720, 136, 100, 100))
anim:AppendFrame(5, Rect(820, 136, 100, 100))
anim:AppendFrame(5, Rect(910, 136, 90, 100))
anim:SetLooping(1)
anim:SetTexture("tex_demon")

sheet = SpriteSheet()
sheet:SetAnim("default", anim)
sheet:SetAnim("walking", anim)

sprite = SpriteRenderer()
sprite:SetSheet(sheet)
sprite:SetAnim("default")

layer:SetTile(0, 0, 1)
layer:SetTile(1, 0, 1)
layer:SetTile(2, 0, 1)
layer:SetTile(3, 0, 1)
layer:SetTile(3, 5, 1)
layer:SetTile(2, 4, 4)
layer:SetTile(5, 6, 5)
layer:SetTile(6, 6, 6)
layer:LoadRegion(0, 0, 32, 32)
layer:SetTexture("tex_tileset_grass")

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
	sprite:Draw(painter)
	sprite:Update()

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
