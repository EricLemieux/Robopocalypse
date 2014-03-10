#version 330 core

in vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

out vec4 finalColour;

void main()
{
	finalColour = vec4(1,1,1,1);
}