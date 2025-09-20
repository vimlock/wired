
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
	float cornerRadius = 0.1;
	vec2 center = vec2(0.5, 0.5);
	vec2 q = abs(vTexcoord - center);

	vec2 rectHalfSize = vec2(0.5, 0.5) - cornerRadius;

	float dist = length(max(q  - rectHalfSize, 0.0f)) - cornerRadius;
	
	float alpha = smoothstep(0.0, 0.01, -dist);

	fColor = uColor * vec4(vec3(1.0), alpha);
}
]])

shader:Compile()

painter = Painter()
painter:SetViewport(0, 0, 800, 600)
painter:SetShader(shader)

painter:Clear(Color(0.1, 0.1, 0.1, 1))

function update()
	painter:PushState()

	painter:SetColor(Color(0.15, 0.15, 0.15, 1.0));
	painter:DrawRect(Rect(400, 100, 400, 300))

	painter:SetColor(Color(0.30, 0.15, 0.15, 1.0));
	painter:DrawRect(Rect(20, 32, 400, 300))

	painter:SetColor(Color(0.15, 0.15, 0.30, 1.0));
	painter:DrawRect(Rect(24, 64, 400, 300))
	painter:PopState()
end
