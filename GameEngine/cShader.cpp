#include "cShaderUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>	// glfw.h, etc. (openGL.h)

cShaderUtils::cShader::cShader()
{
	this->id = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderUtils::cShader::~cShader() 
{
	return;
}

std::string cShaderUtils::cShader::getShaderTypeString(void)
{
	switch ( this->shaderType )
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	// Should never reach here...
	return "UNKNOWN_SHADER_TYPE";
}


//// TODO: For the students to do, because it's FUN, FUN, FUN
//std::map< std::string /*name of uniform variable*/,
//		    unsigned int /* uniform location ID */ > 
//						mapUniformName_to_UniformLocation;

// Look up the uniform inside the shader, then save it, if it finds it
bool cShaderUtils::cShaderProgram::loadUniformLocation(std::string variableName)
{
	// 
	GLint uniLocation = glGetUniformLocation(this->id, 
											 variableName.c_str() );
	// Did it find it (not -1)
	if ( uniLocation == -1 )
	{	// Nope.
		return false;
	}
	// Save it
	this->mapUniformNameToUniformLocation[variableName.c_str()] = uniLocation;

	return true;	
}

// Look up the uniform location and save it.
int cShaderUtils::cShaderProgram::getUniformIdFromName(std::string name)
{
	std::map< std::string /*name of uniform variable*/,
			  int /* uniform location ID */ >::iterator 
		itUniform = this->mapUniformNameToUniformLocation.find(name);

	if ( itUniform == this->mapUniformNameToUniformLocation.end() )
	{
		return -1;		// OpenGL uniform not found value
	}

	return itUniform->second;		// second if the "int" value
}
