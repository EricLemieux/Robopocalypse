#pragma once

#include "VertexBuffer.h"

inline VertexBuffer* ShapeCube(bool useNormals, bool useTexCoords)
{
	VertexBuffer *cube = new VertexBuffer;

	cube->Initialize(36, useNormals, useTexCoords);

	float verts[] = {	-0.5f, -0.5f, +0.5f,	// +z
						+0.5f, -0.5f, +0.5f,
						-0.5f, +0.5f, +0.5f,
						-0.5f, +0.5f, +0.5f,
						+0.5f, -0.5f, +0.5f,
						+0.5f, +0.5f, +0.5f,

						-0.5f, +0.5f, -0.5f,	// -z
						+0.5f, +0.5f, -0.5f,
						-0.5f, -0.5f, -0.5f,
						-0.5f, -0.5f, -0.5f,
						+0.5f, +0.5f, -0.5f,
						+0.5f, -0.5f, -0.5f,

						+0.5f, -0.5f, +0.5f,	// +x
						+0.5f, -0.5f, -0.5f,
						+0.5f, +0.5f, +0.5f,
						+0.5f, +0.5f, +0.5f,
						+0.5f, -0.5f, -0.5f,
						+0.5f, +0.5f, -0.5f,

						-0.5f, -0.5f, -0.5f,	// -x
						-0.5f, -0.5f, +0.5f,
						-0.5f, +0.5f, -0.5f,
						-0.5f, +0.5f, -0.5f,
						-0.5f, -0.5f, +0.5f,
						-0.5f, +0.5f, +0.5f,

						-0.5f, +0.5f, +0.5f,	// +y
						+0.5f, +0.5f, +0.5f,
						-0.5f, +0.5f, -0.5f,
						-0.5f, +0.5f, -0.5f,
						+0.5f, +0.5f, +0.5f,
						+0.5f, +0.5f, -0.5f,

						-0.5f, -0.5f, -0.5f,	// -y
						+0.5f, -0.5f, -0.5f,
						-0.5f, -0.5f, +0.5f,
						-0.5f, -0.5f, +0.5f,
						+0.5f, -0.5f, -0.5f,
						+0.5f, -0.5f, +0.5f,
	};

	cube->AddVerticies(verts);

	if (useNormals)
	{
		float normals[] = { +0.0f, +0.0f, +1.0f,	// +z
							+0.0f, +0.0f, +1.0f,
							+0.0f, +0.0f, +1.0f,
							+0.0f, +0.0f, +1.0f,
							+0.0f, +0.0f, +1.0f,
							+0.0f, +0.0f, +1.0f,
								   
							+0.0f, +0.0f, -1.0f, 	// -z
							+0.0f, +0.0f, -1.0f,
							+0.0f, +0.0f, -1.0f,
							+0.0f, +0.0f, -1.0f,
							+0.0f, +0.0f, -1.0f,
							+0.0f, +0.0f, -1.0f,
								   
							+1.0f, +0.0f, +0.0f, 	// +x
							+1.0f, +0.0f, +0.0f,
							+1.0f, +0.0f, +0.0f,
							+1.0f, +0.0f, +0.0f,
							+1.0f, +0.0f, +0.0f,
							+1.0f, +0.0f, +0.0f,
								   
							-1.0f, +0.0f, +0.0f, 	// -x
							-1.0f, +0.0f, +0.0f,
							-1.0f, +0.0f, +0.0f,
							-1.0f, +0.0f, +0.0f,
							-1.0f, +0.0f, +0.0f,
							-1.0f, +0.0f, +0.0f,
								   
							+0.0f, +1.0f, +0.0f, 	// +y
							+0.0f, +1.0f, +0.0f,
							+0.0f, +1.0f, +0.0f,
							+0.0f, +1.0f, +0.0f,
							+0.0f, +1.0f, +0.0f,
							+0.0f, +1.0f, +0.0f,
							
							+0.0f, -1.0f, +0.0f, 	// -y
							+0.0f, -1.0f, +0.0f,
							+0.0f, -1.0f, +0.0f,
							+0.0f, -1.0f, +0.0f,
							+0.0f, -1.0f, +0.0f,
							+0.0f, -1.0f, +0.0f,
		};

		cube->AddNormals(normals);
	}
	
	if (useTexCoords)
	{
		float texCoords[] = {	0.375f, 0.00f,			// +z
								0.625f, 0.00f,
								0.375f, 0.25f,
								0.375f, 0.25f,
								0.625f, 0.00f,
								0.625f, 0.25f,

								0.375f, 0.50f,			// -z
								0.625f, 0.50f,
								0.375f, 0.75f,
								0.375f, 0.75f,
								0.625f, 0.50f,
								0.625f, 0.75f,

								0.625f, 0.00f,			// +x
								0.875f, 0.00f,
								0.625f, 0.25f,
								0.625f, 0.25f,
								0.875f, 0.00f,
								0.875f, 0.25f,

								0.125f, 0.00f,			// -x
								0.375f, 0.00f,
								0.125f, 0.25f,
								0.125f, 0.25f,
								0.375f, 0.00f,
								0.375f, 0.25f,

								0.375f, 0.25f,			// +y
								0.625f, 0.25f,
								0.375f, 0.50f,
								0.375f, 0.50f,
								0.625f, 0.25f,
								0.625f, 0.50f,

								0.375f, 0.75f,			// -y
								0.625f, 0.75f,
								0.375f, 1.00f,
								0.375f, 1.00f,
								0.625f, 0.75f,
								0.625f, 1.00f, 
		};

		cube->AddTexCoords(texCoords);
	}
	
	return cube;
}

inline VertexBuffer* ShapeFullScreenQuad(void)
{
	VertexBuffer *quad = new VertexBuffer;
	
	float verts[]	= { -1.0f, -1.0f, -1.0f, 
						 1.0f, -1.0f, -1.0f, 
						-1.0f,  1.0f, -1.0f, 
						-1.0f,  1.0f, -1.0f, 
						 1.0f, -1.0f, -1.0f, 
						 1.0f,  1.0f, -1.0f, 
	};

	float texCoords[]= {
						0.0f, 0.0f, 
						1.0f, 0.0f, 
						0.0f, 1.0f, 
						0.0f, 1.0f, 
						1.0f, 0.0f, 
						1.0f, 1.0f, 
	};

	quad->Initialize(6, false, true);
	quad->AddVerticies(verts);
	quad->AddTexCoords(texCoords);

	return quad;
}

inline VertexBuffer* ShapeHUDQuad(float width, float height)
{
	VertexBuffer *quad = new VertexBuffer;

	quad->Initialize(6, false, true);
	if (width >= 1.0f || height >= 1.0f)
	{
		width	= width / glm::max(width, height);
		height	= height / glm::max(width, height);
	}
	
	float verts[] = {	-1.0f*width,	-1.0f*height,	-1.0f,
						+1.0f*width,	-1.0f*height,	-1.0f,
						-1.0f*width,	+1.0f*height,	-1.0f,
						-1.0f*width,	+1.0f*height,	-1.0f,
						+1.0f*width,	-1.0f*height,	-1.0f,
						+1.0f*width,	+1.0f*height,	-1.0f,
	};

	float texCoords[] = {	0.0f, 0.0f,
							1.0f, 0.0f,
							0.0f, 1.0f,
							0.0f, 1.0f,
							1.0f, 0.0f,
							1.0f, 1.0f,
	};

	quad->AddVerticies(verts);
	quad->AddTexCoords(texCoords);

	return quad;
}

inline VertexBuffer* ShapeHUDQuad(float width, float height, float percentage, int faceDirection)
{
	percentage = glm::clamp(percentage, 0.0f, 1.0f);

	VertexBuffer *quad = new VertexBuffer;

	quad->Initialize(6, false, true);
	if (width >= 1.0f || height >= 1.0f)
	{
		width = width / glm::max(width, height);
		height = height / glm::max(width, height);
	}

	float verts[] = {	-1.0f*faceDirection*width*percentage, -1.0f*height, -1.0f,
						+0.0f*faceDirection*width, -1.0f*height, -1.0f,
						-1.0f*faceDirection*width*percentage, +1.0f*height, -1.0f,
						-1.0f*faceDirection*width*percentage, +1.0f*height, -1.0f,
						+0.0f*faceDirection*width, -1.0f*height, -1.0f,
						+0.0f*faceDirection*width, +1.0f*height, -1.0f,
	};

	float texCoords[] = { 0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	quad->AddVerticies(verts);
	quad->AddTexCoords(texCoords);

	return quad;
}