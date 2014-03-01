#include "Texture.h"


Texture::Texture()
{
}
Texture::Texture(char* filePath)
{
	Load(filePath);
}


Texture::~Texture()
{
}

int Texture::Load(char* filePath)
{
	//ILuint tex;
	//ilGenImages(1, &tex);
	//ilBindImage(tex);
	//
	//ilLoadImage(filePath);
	//ILubyte *bytes = ilGetData();
	//
	////If the image is empty return an error
	//if (!bytes)
	//{
	//	std::cout << "Error opening the file: " << filePath << std::endl;
	//
	//	//Clean up memory
	//	ilBindImage(0);
	//	ilDeleteImages(1, &tex);
	//
	//	//Return a zero to say it didnt work.
	//	return 0;
	//}
	////Image loaded properly and can generate a handle for openGL
	//else
	//{
	//	glGenTextures(1, &handle);
	//	glBindTexture(GL_TEXTURE_2D, handle);
	//
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	//
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//
	//	//Delete the DevIL image to save space because we already have the image ready for use
	//	ilBindImage(0);
	//	ilDeleteImages(1, &tex);
	//
	//	//Return 1 to show that texture loading worked
	//	return 1;
	//}

	handle = ilutGLLoadImage(filePath);

	return 1;
}

GLuint Texture::GetHandle(void)
{
	if(handle)
		return handle;
	return 0;
}