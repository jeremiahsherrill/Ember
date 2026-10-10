#version 450

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec4 a_Color;

layout(push_constant) uniform PushConstants {
	vec2 u_Scale;
	vec2 u_Translate;
} pc;

layout(location = 0) out vec2 v_UV;
layout(location = 1) out vec4 v_Color;

void main()
{
	v_UV = a_UV;
	v_Color = a_Color;
	gl_Position = vec4(a_Position * pc.u_Scale + pc.u_Translate, 0.0, 1.0);
}
