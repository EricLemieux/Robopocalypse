#include "GLSL.h"


//////////
//GLSLShader
//////////

//Constructor
GLSLShader::GLSLShader(void)
{
	handle = 0;
}

//Destructor
GLSLShader::~GLSLShader(void)
{
	Release();
}

//Release
void GLSLShader::Release(void)
{
	if(handle)
	{
		glDeleteShader(handle);
		handle = 0;
	}
}

//Create the shader
int GLSLShader::CreateShaderFromString(shaderType newType, const char *source)
{
	//only create shader if one does not already exist
	if(!handle)
	{
		//Make sure there is something in the string to load
		if(source)
		{
			//changes our internal shader type to the type for use by openGL
			const GLenum internalType[6] = 
			{
				GL_VERTEX_SHADER,
				GL_FRAGMENT_SHADER,
			};

			//create the shader in graphics memory
			type = newType;
			handle = glCreateShader(internalType[type]);

			//if handle was created sucessfuly
			if(handle)
			{
				int status;

				glShaderSource(handle, 1, &source, 0);

				glCompileShader(handle);

				glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
				
				//if the shader is compliled successfully
				if(status)
				{
					std::cout<<"\nShader compiled successfully.\n";
					return handle;
				}
				//if the shader was not compiled successfully output the error message
				int logLength;
				glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
				if(logLength > 0)
				{
					char log[sizeof(logLength) + 1];
					glGetShaderInfoLog(handle, logLength, &logLength, log);
					std::cout<<"\nShader failed to complile:\n"<<log<<std::endl;
				}

				//Finally release the failed shader from graphics memory
				Release();
			}
		}
	}
}

//Create shader using a file
int GLSLShader::CreateShaderFromFile(shaderType newType, const char *filePath)
{
	std::ifstream file;
	file.open(filePath, std::ifstream::in);

	if(file == NULL)
	{
		std::cout<<"\n Error reading file.\n";
	}

	//find size of the file
	file.seekg(0, std::ios::end);
	const static int size = file.tellg();
	file.seekg(0);

	char* source;
	source = (char *) malloc (size);
	unsigned int i = 0;

	while(!file.eof())
	{
		char c = file.get();
		source[i] = c;
		++i;
	}
	source[i-1] = '\0';

	std::cout<<source;
	int ret = CreateShaderFromString(newType, source);
	if(&source)
	{
		delete source;
	}
	return ret;
}

//////////
//GLSL Program class
//////////

//Constructor
GLSLProgram::GLSLProgram(void)
{
	handle = 0;
}

//Destructor
GLSLProgram::~GLSLProgram(void)
{
	Release();
}

//Release the program from memory
void GLSLProgram::Release(void)
{
	//if the handle actually exists
	if(handle)
	{
		glDeleteProgram(handle);
		handle = 0;
	}
}

//Attach the shader to the program
int GLSLProgram:: AttachShader(GLSLShader *shader)
{
	//only attach if there is a shader there
	if(shader)
	{
		//If the handle has not yet been created, create one now
		if(!handle)
		{
			handle = glCreateProgram();
		}

		//Now if the handle exists attacht the shader to the program.
		if(handle)
		{
			glAttachShader(this->handle, shader->GetHandle());	//No idea why this doesnt work...	//TODO Fix
			return 1;
		}
	}
	return 0;
}

//Link the program
int GLSLProgram::LinkProgram(void)
{
	if(handle)
	{
		int status;
		glLinkProgram(handle);
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		if(status)
		{
			std::cout<<"\nProgram linkled successfully\n";
			return 1;
		}

		int logLength;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0)
		{
			char *log = new char[];
			glGetProgramInfoLog(handle, logLength, &logLength, log);
			std::cout<<"\nError in shader compile:\n"<<log<<std::endl;
			delete log;
		}

		Release();
	}
	return 0;
}

//Validation
int GLSLProgram::ValidateProgram(void)
{
	if(handle)
	{
		glValidateProgram(handle);
		return handle;
	}
	return 0;
}

//Activate
void GLSLProgram::Activate(void)
{
	glUseProgram(handle);
}

//Deactivate
void GLSLProgram::Deactivate(void)
{
	glUseProgram(0);
}

//get Uniform
int GLSLProgram::GetUniformLocation(const char *uniformName)
{
	return glGetUniformLocation(handle, uniformName);
}

//Send uniform
void GLSLProgram::UniformFloat(int location, float value)
{

	glUniform1f(location, value);
}

void GLSLProgram::UniformMat4(int location, bool transpose, const glm::mat4 value)
{
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
}

//Shits broke
//TODO: fix it
//void GLSLProgram::UniformVec3List(int location, unsigned int numValues, const glm::vec3 *valueList)
//{
//	glUniform3f(location, numValues, valueList);
//}