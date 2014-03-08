#version 330 core

uniform sampler2D objectTexture;
uniform vec2 flipDirection;

in vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

out vec4 finalColour;

void main()
{
	finalColour = texture(objectTexture, flipDirection * data.UV);
}