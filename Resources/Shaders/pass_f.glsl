#version 330 core

uniform sampler2D objectTexture;

varying vec2 UV;
varying vec4 COL;

out vec4 finalColour;

void main()
{
	//COL;//vec4(UV.x, UV.y, 0, 1);//texture(objectTexture, UV);

	finalColour = texture(objectTexture, UV);
}