#version 330 core

layout(location = 0) in vec4 position;
layout(location = 2) in vec3 normal;
layout(location = 8) in vec2 texcoord;

uniform mat4 MVP;

out vertex
{
	vec3 positionObj;
	vec3 normalObj;
	vec2 texcoordObj;
}data;

void main()
{
	gl_Position = MVP * position;

	data.positionObj	= position.xyz;
	data.normalObj		= normal;
	data.texcoordObj	= texcoord;
}