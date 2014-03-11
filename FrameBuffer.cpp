#include "FrameBuffer.h"

#include <string>

FrameBuffer::FrameBuffer(void)
{
	handle			= 0;

	memset(colourTex, 0, sizeof(colourTex));
	
	numTargets		= 0;

	depthTex		= 0;

	width = height	= 0;

	maxSize			= 4096;
}


FrameBuffer::~FrameBuffer(void)
{
	//Release everything from graphics memory
	Release();
}


void FrameBuffer::Release(void)
{
	//Can only release data if there is a handle to get the data
	if(handle)
	{
		//Delete the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &handle);

		//Delete the textures
		glBindTexture(GL_TEXTURE_2D, 0);
		if(*colourTex)
		{
			glDeleteTextures(1, colourTex);
		}

		//Reset the values
		handle			= 0;
		memset(colourTex, 0, sizeof(colourTex));
		numTargets		= 0;
		depthTex		= 0;
		width = height	= 0;
	}
}

int FrameBuffer::Initialize(unsigned int w, unsigned int h,
							unsigned int colour, bool useDepth, bool useHDR,
							bool useLinearFilter, bool clamp)
{
	if(w && h && (colour || useDepth))
	{
		if (w < maxSize && w < maxSize)
		{
			width = w;
			height = h;

			const unsigned short filtering = useLinearFilter ?
			GL_LINEAR : GL_NEAREST;
			const unsigned short wrapping = clamp ?
			GL_CLAMP_TO_EDGE : GL_REPEAT;
			const unsigned short colourBit = useHDR ?
			GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;
			const unsigned short colourFormat = useHDR ?
			GL_RGBA16 : GL_RGBA8;

			glGenFramebuffers(1, &handle);
			glBindFramebuffer(GL_FRAMEBUFFER, handle);

			if (colour)
			{
				//Get the maximum number of targets aloud
				int maxTargets;
				glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxTargets);

				//Detremine the number of targets this FBO will have
				if (colour > maxTargets)
					numTargets = maxTargets;
				else
					numTargets = colour;

				glGenTextures(numTargets, colourTex);

				for (unsigned int i = 0; i < numTargets; ++i)
				{
					glBindTexture(GL_TEXTURE_2D, *colourTex + i);

					glTexImage2D(GL_TEXTURE_2D, 0, colourFormat, width, height, 0, GL_RGBA, colourBit, 0);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

					glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, *colourTex + i, 0);
				}
			}

			if (useDepth)
			{
				glGenTextures(1, &depthTex);
				glBindTexture(GL_TEXTURE_2D, depthTex);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

				//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);
			}

			return true;
		}
	}

	return false;
}

void FrameBuffer::Activate() const
{
	//If the FBO has been created and has a handle
	if(handle)
	{
		//Bind the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, handle);

		if(numTargets)
		{
			const GLenum targetName[16] = {
				GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2,
				GL_COLOR_ATTACHMENT3,
				GL_COLOR_ATTACHMENT4,
				GL_COLOR_ATTACHMENT5,
				GL_COLOR_ATTACHMENT6,
				GL_COLOR_ATTACHMENT7,
				GL_COLOR_ATTACHMENT8,
				GL_COLOR_ATTACHMENT9,
				GL_COLOR_ATTACHMENT10,
				GL_COLOR_ATTACHMENT11,
				GL_COLOR_ATTACHMENT12,
				GL_COLOR_ATTACHMENT13,
				GL_COLOR_ATTACHMENT14,
				GL_COLOR_ATTACHMENT15,
			};

			glDrawBuffers(numTargets, targetName);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}

		if(depthTex)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
}

void FrameBuffer::Deactivate(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDrawBuffer(GL_FRONT);

	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::UnbindTextures(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

//////////
//SETTERS
//////////

void FrameBuffer::BindColour(unsigned int target) const
{
	if(target < 16)
	{
		if(numTargets)
		{
			glBindTexture(GL_TEXTURE_2D, *(colourTex + target));
		}
	}
}

void FrameBuffer::BindDepth() const
{
	if(depthTex)
	{
		glBindTexture(GL_TEXTURE_2D, depthTex);
	}
}

void FrameBuffer::SetTexture(unsigned int texture)
{
	if(texture < 16)
	{
		glActiveTexture(GL_TEXTURE0 + texture);
	}
}