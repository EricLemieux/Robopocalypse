#version 330 core

in vertex
{
	vec2 uv;
}data;

uniform sampler2D objectTexture;

layout (location=0) out vec4 colour;

void main()
{

	vec4 diffuse	= texture(objectTexture, data.uv);

	colour = diffuse;
}