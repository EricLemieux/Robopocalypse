#version 330 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

out vertex{
	vec2 uv;
}data;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 distortion;
uniform float squareSize;

void main(){

	vec4 positionObj = gl_in[0].gl_Position;
	vec4 positionEye = modelview * positionObj;


	vec4 offset = vec4(0.0);

	//gl_Position = projection * positionEye;
	//data.colour = vec4(1.0, 0.0, 0.0, 1.0);
	//EmitVertex();

	offset.x = -squareSize;
	offset.y = -squareSize;

	gl_Position = projection * (positionEye + offset);
	data.uv = vec2(0.0);
	EmitVertex();

	offset.x = squareSize;
	gl_Position = projection * (positionEye + offset);
	data.uv.x = 1.0;	
	EmitVertex();

	offset.x = -squareSize;
	offset.y = +squareSize;
	gl_Position = projection * (positionEye + offset);
	data.uv = vec2(0.0,1.0);	
	EmitVertex();

	offset.x = squareSize;
	gl_Position = projection * (positionEye + offset);
	data.uv.x = 1.0;	
	EmitVertex();

	EndPrimitive();
}

//#version 330 core
//
//layout (points) in;
//layout (points, max_vertices = 1) out;
//
//
//void main()
//{
//	gl_Position		= gl_in[0].gl_Position;
//	EmitVertex();				  
//
//}