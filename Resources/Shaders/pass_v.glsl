#version 330 core

layout (location=0) in vec4 position;
layout (location=2) in vec3 normal;
layout (location=8) in vec2 texcoord;

uniform mat4 MVP;

out vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

void main()
{
	gl_Position = MVP * position;

	data.pos	= gl_Position.xyz;
	data.norm	= normal;
	data.UV		= texcoord;
}