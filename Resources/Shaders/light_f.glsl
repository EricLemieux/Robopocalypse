#version 330 core

in vertex
{
	vec3 pos;
}data;

layout (location=0) uniform sampler2D g_diffuse;
layout (location=1) uniform sampler2D g_normals;
layout (location=3) uniform sampler2D g_depth;

layout (location=4) uniform sampler2D qMap;

uniform mat4 inverseP;

vec3 lightColour = vec3(1.0, 0.0, 0.0);
vec3 lightPos = data.pos;

layout (location=0) out vec4 finalColour;

vec3 light(in vec3 pos, in vec3 norm, in vec3 sceneDiffuse)
{
	vec3 N = normalize(norm);
	vec3 L = normalize(lightPos - pos);

	float lambert = max(0.0, dot(N,L));

	lambert = texture(qMap, vec2(lambert,0.0)).r;
	
	vec3 diffuse = lambert * lightColour * sceneDiffuse;

	return diffuse;
}

void main()
{
	vec2 screenLocation = gl_FragCoord.xy/vec2(1280.0,720.0);

	vec3 sceneDiffuse	= texture(g_diffuse, screenLocation).rgb;
	//vec3 sceneNormals	= texture(g_normals, screenLocation).rgb;
	float depthValue	= texture(g_depth, screenLocation);

	//vec4 position =  inverseMVP * vec4(screenLocation, depthValue, 1.0);
	//position *= 1/position.w;
	vec3 position = inverseP * vec4(screenLocation, depthValue,1.0);
	
	vec3 normal = texture(g_normals, screenLocation).rgb;//normalize(texture(g_normals, screenLocation).rgb * 2.0 - 1.0);

	vec3 lightValue = light(position.rgb, normal, sceneDiffuse);

	//finalColour.rgb = lightValue /*+ sceneDiffuse*/;
	finalColour.rgb = depthValue;
}