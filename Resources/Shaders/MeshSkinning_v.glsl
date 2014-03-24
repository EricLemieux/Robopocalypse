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

void skinVertex(in vec4 vPos, in vec3 vNorm,
				out vec4 newPos, out vec3 newNorm,
				in vec4 vertexWeight, in vec4 vertexIndex)
{
	vec4 weightedPos	= vec4(0.0);
	vec3 weightedNorm	= vec3(0.0);

	vec4 normal			= vec4(vNorm, 0.0);

	for(uint i = 0; i < 4; ++i)
	{
		weightedPos		+= (boneMatricies[int(vertexIndex[i])] * vPos)		* vertexWeight[i];
		weightedNorm	+= (boneMatricies[int(vertexIndex[i])] * normal)	* vertexWeight[i];
	}

	newPos		= weightedPos;
	newNorm		= weightedNorm.xyz;
}

void main()
{
	vec4 skinPos;
	vec4 skinNorm;

	skinVertex(position, normal, skinPos, skinNorm, boneWeights, boneIndex);

	gl_Position = MVP * skinPos;

	data.pos	= skinPos.xyz;
	data.norm	= skinNorm.xyz;
	data.UV		= texcoord;
}