#version 330 core

in vec4 position;

uniform mat4 MVP;

void main()
{
	glPosition = MVP * position;
}