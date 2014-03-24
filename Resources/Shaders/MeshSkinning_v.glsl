#version 330 core

#define MAX_BONES 64

layout (location=0)		in vec4		position;
layout (location=2)		in vec3		normal;
layout (location=8)		in vec2		texcoord;
layout (location=10)	in vec4		boneIndex;
layout (location=11)	in vec4		boneWeights;

uniform mat4 MVP;
uniform mat4 boneMatricies[MAX_BONES];

out vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

void main()
{
	mat4 jointMat;
	for(uint i = 0; i < 4; ++i)
	{
		jointMat += boneMatricies[int(boneIndex[i])] * boneWeights[i];
	}

	gl_Position = MVP * jointMat * position;

	data.pos	= position.xyz;
	data.norm	= normal;
	data.UV		= texcoord;
}