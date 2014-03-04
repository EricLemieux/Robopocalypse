#version 330 core

uniform sampler2D objectTexture;
uniform vec2 flipDirection;

varying vec2 UV;
varying vec4 COL;

out vec4 finalColour;

void main()
{
	finalColour = texture(objectTexture, flipDirection * UV);
}