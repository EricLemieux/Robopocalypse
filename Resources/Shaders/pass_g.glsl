#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data_in[];

out vertex
{
	vec3 pos;
	vec3 norm;
	vec2 UV;
}data_out;

void main()
{
	gl_Position		= gl_in[0].gl_Position;
	data_out.pos	= data_in[0].pos;
	data_out.norm	= data_in[0].norm;
	data_out.UV		= data_in[0].UV;
	EmitVertex();				  

	gl_Position		= gl_in[1].gl_Position;
	data_out.pos	= data_in[1].pos;
	data_out.norm	= data_in[1].norm;
	data_out.UV		= data_in[1].UV;
	EmitVertex();				  

	gl_Position		= gl_in[2].gl_Position;
	data_out.pos	= data_in[2].pos;
	data_out.norm	= data_in[2].norm;
	data_out.UV		= data_in[2].UV;
	EmitVertex();
}