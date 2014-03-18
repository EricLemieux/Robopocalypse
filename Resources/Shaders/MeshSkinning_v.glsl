#version 330 core

layout (location=0)		in vec4		position;
layout (location=2)		in vec3		normal;
layout (location=8)		in vec2		texcoord;
layout (location=10)	in float	boneIndex[4];
layout (location=11)	in float	boneWeights[4];

uniform mat4 MVP;
uniform mat4 boneMatricies[64];

out vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

void main()
{
	//Doing this way because for loops are too slow
	//mat4 jointMat = boneMatricies[int(boneIndex[0])] * boneWeights[0] + boneMatricies[int(boneIndex[1])] * boneWeights[1] + boneMatricies[int(boneIndex[2])] * boneWeights[2] + boneMatricies[int(boneIndex[3])] * boneWeights[3];
	mat4 jointMat = mat4(boneIndex[0]);
	//mat4 jointMat = mat4(1.0);
	
	gl_Position = MVP * jointMat * position;

	data.pos	= position.xyz;
	data.norm	= normal;
	data.UV		= texcoord;
}