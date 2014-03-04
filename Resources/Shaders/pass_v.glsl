#version 330 core

layout (location=0) in vec4 position;
layout (location=8) in vec2 texcoord;

uniform mat4 MVP;

varying vec2 UV;
varying vec4 COL;

void main()
{
	gl_Position = MVP * position;
	COL = vec4(1,0,0,1);
	UV = texcoord;
}