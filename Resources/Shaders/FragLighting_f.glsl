#version 330 core

in vertex
{
	vec3 positionObj;
	vec3 normalObj;
	vec2 texcoordObj;
}data;

uniform vec3 lightPos;

uniform sampler2D objectTexture;

out vec4 finalColour;

vec3 light(in vec3 pos, in vec3 norm)
{
	vec3 N = normalize(norm);
	vec3 L = normalize(lightPos - pos);

	float lambert = max(0.0, dot(N,L));

	vec3 lightColour = vec3(1.0, 1.0, 1.0);
	vec3 objectColour = vec3(1.0, 0.0, 0.0);
	
	vec3 diffuse = lambert * lightColour * texture(objectTexture,data.texcoordObj).rgb;

	//TEMP
	diffuse.rgb = texture(objectTexture, data.texcoordObj).rgb;

	return diffuse;
}

void main()
{
	finalColour = vec4(0.0, 0.0, 0.0, 1.0);
	finalColour.rgb = light(data.positionObj, data.normalObj);
}