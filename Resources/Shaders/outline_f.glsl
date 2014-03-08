#version 330 core

in vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

uniform sampler2D scene;

out vec4 finalColour;

void main()
{
	finalColour.rgb = texture(scene, data.UV).rgb;
}