#version 330 core

in vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data;

uniform sampler2D scene;

out vec4 finalColour;

vec3 kernel(in vec2 tex, in sampler2D image, in mat3 kernel);

void main()
{
	mat3 edgeDetect = mat3(	0,1,0
							,1,-4,1
							,0,1,0);
	
	vec3 outline = kernel(data.UV, scene, edgeDetect);
	outline = vec3(max(outline.r, max(outline.g, outline.b)));

	vec3 diffuse = texture(scene, data.UV).rgb;

	finalColour.rgb = (1.0-outline) * diffuse;
}

vec3 kernel(in vec2 tex, in sampler2D image, in mat3 kernel)
{
	vec3 outputValue = vec3(0);

	float size = 0.002;

	outputValue += kernel[0].x * texture(image, tex + (vec2(-1, 1) * size)).rgb;
	outputValue += kernel[0].y * texture(image, tex + (vec2(-1, 0) * size)).rgb;
	outputValue += kernel[0].z * texture(image, tex + (vec2(-1,-1) * size)).rgb;
												
	outputValue += kernel[1].x * texture(image, tex + (vec2( 0, 1) * size)).rgb;
	outputValue += kernel[1].y * texture(image, tex + (vec2( 0, 0) * size)).rgb;
	outputValue += kernel[1].z * texture(image, tex + (vec2( 0,-1) * size)).rgb;
														
	outputValue += kernel[2].x * texture(image, tex + (vec2( 1, 1) * size)).rgb;
	outputValue += kernel[2].y * texture(image, tex + (vec2( 1, 0) * size)).rgb;
	outputValue += kernel[2].z * texture(image, tex + (vec2( 1,-1) * size)).rgb;

	return outputValue;
}