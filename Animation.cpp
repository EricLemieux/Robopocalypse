#include "Animation.h"


Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::LoadSkinWeights(void)
{
	//LOAD IMAGE AND LOOK AT VALUES
	ILuint *tex;
	tex = new ILuint;
	ilGenImages(1, tex);
	ilBindImage(*tex);

	ilLoadImage("Resources/NormalMaps/blank.png");
	ILubyte* data = ilGetData();

	ILuint width, height;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	glm::vec3 pixelData;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pixelData.r = (data[(i*width + j) * 4 + 0]);
			pixelData.g = (data[(i*width + j) * 4 + 1]);
			pixelData.b = (data[(i*width + j) * 4 + 2]);
		}
	}
}