#version 450

layout(location = 0) in vec2 v_UV;
layout(location = 1) in vec4 v_Color;

layout(set = 0, binding = 0) uniform sampler2D u_FontTexture;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = v_Color * texture(u_FontTexture, v_UV);
}
