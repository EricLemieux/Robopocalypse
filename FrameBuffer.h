#pragma once

#include <GL\glew.h>

class FrameBuffer
{
public:
	FrameBuffer(void);
	~FrameBuffer(void);

	void Release(void);

	int Initialize(unsigned int w, unsigned int h,
					unsigned int colour, bool useDepth=true, 
					bool useHDR=true, bool useLinearFilter=true, bool clamp=true);

	void Activate(void) const;
	static void Deactivate(void);

	void UnbindTextures(void);

	//////////
	//SETTERS
	//////////

	void BindColour(unsigned int target = 0) const;

	void BindDepth(void) const;

	void SetTexture(unsigned int texture = 0);

	//////////
	//GETTERS
	//////////

	//Get the handle
	inline unsigned int GetHandle(void)		{ return handle;	}

	//Get the texture handles
	inline unsigned int* GetColourTex(void)	{ return colourTex;	}
	inline unsigned int GetNumTargets(void) { return numTargets;}

	//Get the dimentions of the frame buffer
	inline unsigned int GetWidth(void)		{ return width;		}
	inline unsigned int GetHeight(void)		{ return height;	}

private:
	//Handle for openGl access 
	unsigned int handle;

	//Texture handle
	unsigned int colourTex[16];
	unsigned int numTargets;

	//Depth target
	unsigned int depthTex;

	//Dimentions of the Frame Buffer
	unsigned int width, height;

	//max dimension
	unsigned int maxSize;
};

