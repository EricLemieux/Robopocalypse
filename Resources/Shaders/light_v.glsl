#version 330 core

layout (location=0) in vec4 position;

uniform mat4 MVP;

out vertex
{
	vec3 pos;
}data;

void main()
{
	gl_Position = MVP * position;

	data.pos	= position.xyz;
}