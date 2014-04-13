#version 330 core

in vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

uniform sampler2D objectTexture;
uniform sampler2D objectNormalMap;

layout (location=0) out vec4 finalColour;
layout (location=1) out vec3 finalNormals;
layout (location=2) out vec3 finalDepth;

void main()
{
	//vec3 normal		= (texture(objectNormalMap, data.UV).rgb * 2.0) - 1.0;

	vec3 diffuse	= texture(objectTexture, data.UV).rgb;

	finalColour.rgb = diffuse;
	finalNormals	= data.norm;
	finalDepth		= vec3(data.pos.z);
}