#include "cShaderUtils.h"

#include <glad/glad.h>	// For all the OpenGL calls, etc.

#include <iostream>
#include <fstream>
#include <sstream>		// "string stream"
#include <vector>

#include <algorithm>		// for the std::copy
#include <iterator>			// for the back_inserter
#include "cJsonUtils.h"

cShaderUtils* cShaderUtils::pShaderUtils = NULL;

cShaderUtils::cShaderUtils()
{
	return;
}

cShaderUtils::~cShaderUtils()
{
	return;
}

cShaderUtils* cShaderUtils::getInstance(void)
{
	if (cShaderUtils::pShaderUtils == NULL)
	{
		pShaderUtils = new cShaderUtils();
	}
	return pShaderUtils;
}

GLuint cShaderUtils::setUpShaders()
{
	cShaderUtils* pShaderUtils = getInstance();

	pShaderUtils->setBasePath(cJsonUtils::getJsonInstance()["shaderBasePath"]);

	cShader vertexShader;
	cShader fragmentShader;

	vertexShader.fileName = cJsonUtils::getJsonInstance()["vertexShaderFileName"].get<std::string>();
	vertexShader.shaderType = cShader::VERTEX_SHADER;

	fragmentShader.fileName = cJsonUtils::getJsonInstance()["fragmentShaderFileName"].get<std::string>();
	fragmentShader.shaderType = cShader::FRAGMENT_SHADER;

	std::string shaderProgramName = cJsonUtils::getJsonInstance()["shaderProgramName"].get<std::string>();
	if (pShaderUtils->createProgramFromFile(shaderProgramName,
		vertexShader,
		fragmentShader))
	{
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderUtils->getLastError() << std::endl;
	}

	// Load the uniform location values (some of them, anyway)
	cShaderUtils::cShaderProgram* pSP = cShaderUtils::getInstance()->getShaderProgramFromFriendlyName(shaderProgramName);
	pSP->loadUniformLocation("texture00");
	pSP->loadUniformLocation("texture01");
	pSP->loadUniformLocation("texture02");
	pSP->loadUniformLocation("texture03");
	pSP->loadUniformLocation("texture04");
	pSP->loadUniformLocation("texture05");
	pSP->loadUniformLocation("texture06");
	pSP->loadUniformLocation("texture07");
	pSP->loadUniformLocation("texBlendWeights[0]");
	pSP->loadUniformLocation("texBlendWeights[1]");

	GLuint program = pShaderUtils->getIdFromFriendlyName(shaderProgramName);

	return program;
}

bool cShaderUtils::useShaderProgram(unsigned int ID)
{
	// Use the number directy... 
	// TODO: Might do a lookup to see if we really have that ID...
	glUseProgram(ID);
	return true;
}

bool cShaderUtils::useShaderProgram(std::string friendlyName)
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator
		itShad = this->mFriendlyNameToId.find(friendlyName);

	if (itShad == this->mFriendlyNameToId.end())
	{	// Didn't find it
		// Maybe set glUseProgram(0)....?
		return false;
	}
	glUseProgram(itShad->second);

	return true;
}

unsigned int cShaderUtils::getIdFromFriendlyName(std::string friendlyName)
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator
		itShad = this->mFriendlyNameToId.find(friendlyName);

	if (itShad == this->mFriendlyNameToId.end())
	{	// Didn't find it
		return 0;
	}
	return itShad->second;
}

cShaderUtils::cShaderProgram*
cShaderUtils::getShaderProgramFromFriendlyName(std::string friendlyName)
{
	unsigned int shaderID = this->getIdFromFriendlyName(friendlyName);

	// Now get the actual shader

	std::map< unsigned int /*ID*/, cShaderProgram >::iterator
		itShad = this->mIdToShaderProgram.find(shaderID);

	if (itShad == this->mIdToShaderProgram.end())
	{	// Didn't find it
		return NULL;		// or 0 or nullptr
	}

	cShaderProgram* pShaderIFound = &(itShad->second);

	return pShaderIFound;
}


const unsigned int MAXLINELENGTH = 65536;		// 16x1024

void cShaderUtils::setBasePath(std::string basepath)
{
	this->mBasePath = basepath;
	return;
}


// Returns bool if didn't load
bool cShaderUtils::mLoadSourceFromFile(cShader &shader)
{
	std::string fullFileName = this->mBasePath + shader.fileName;

	std::ifstream theFile(fullFileName.c_str());
	if (!theFile.is_open())
	{
		return false;
	}

	shader.vecSource.clear();

	char pLineTemp[MAXLINELENGTH] = { 0 };
	while (theFile.getline(pLineTemp, MAXLINELENGTH))
	{
		std::string tempString(pLineTemp);
		shader.vecSource.push_back(tempString);
	}

	theFile.close();
	return true;		
}

// Returns empty string if no error
// returns false if no error
bool cShaderUtils::mWasThereACompileError(unsigned int shaderID,
	std::string &errorText)
{
	errorText = "";	// No error

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->mLastError.append("\n");
		this->mLastError.append(errorText);


		delete[] pLogText;	// Oops

		return true;	// There WAS an error
	}
	return false; // There WASN'T an error
}

bool cShaderUtils::mWasThereALinkError(unsigned int programID, std::string &errorText)
{
	errorText = "";	// No error

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if (wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->mLastError.append("\n");
		this->mLastError.append(errorText);


		delete[] pLogText;

		// There WAS an error
		return true;
	}

	// There WASN'T an error
	return false;
}


std::string cShaderUtils::getLastError(void)
{
	std::string lastErrorTemp = this->mLastError;
	this->mLastError = "";
	return lastErrorTemp;
}

GLuint cShaderUtils::getUniformVariableLocation(GLuint program, std::string name)
{
	return glGetUniformLocation(program, name.c_str());
}

#include <iostream>

bool cShaderUtils::mCompileShaderFromSource(cShaderUtils::cShader &shader, std::string &error)
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	// This is an array of pointers to strings. aka the lines of source
	char** arraySource = new char*[numberOfLines];
	// Clear array to all zeros (i.e. '\0' or null terminator)
	memset(arraySource, 0, numberOfLines);

	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
		memset(arraySource[indexLine], 0, numCharacters + 2);

		// Copy line of source into array
		for (unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++)
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}//for ( unsigned int indexChar = 0...

		// At a '\0' at end (just in case)
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';

		// Or you could use the (unsecure) strcpy (string copy)
//		strcpy( arraySource[indexLine], shader.vecSource[indexLine].c_str() );

		// Or the "secure" version, where you specify the number of chars to copy
//		strcpy_s( arraySource[indexLine], 
//				  strlen( shader.vecSource[indexLine].c_str() + 1 ),	// Number of char in copy + 0 terminator
//				  shader.vecSource[indexLine].c_str() );

	}//for ( unsigned int indexLine = 0...

//******************************************************************************************
//	// Print it out (for debug)
//	std::cout << "Source (from char** array):" << std::endl;
//	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
//	{
//		std::cout << "(" << indexLine << "):\t" << arraySource[indexLine] << std::endl;
//	}
//	std::cout << "END" << std::endl;
//******************************************************************************************

	//const char* tempVertChar = "Hey there;";

	glShaderSource(shader.id, numberOfLines, arraySource, NULL);
	glCompileShader(shader.id);

	// Get rid of the temp source "c" style array
	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{	// Delete this line
		delete[] arraySource[indexLine];
	}
	// And delete the original char** array
	delete[] arraySource;

	// Did it work? 
	std::string errorText = "";
	if (this->mWasThereACompileError(shader.id, errorText))
	{
		std::stringstream ssError;
		ssError << shader.getShaderTypeString();
		ssError << " compile error: ";
		ssError << errorText;
		error = ssError.str();
		return false;
	}

	return true;
}



bool cShaderUtils::createProgramFromFile(
	std::string friendlyName,
	cShader &vertexShad,
	cShader &fragShader)
{
	std::string errorText = "";


	// Shader loading happening before vertex buffer array
	vertexShad.id = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;
	//  char* vertex_shader_text = "wewherlkherlkh";
	// Load some text from a file...
	if (!this->mLoadSourceFromFile(vertexShad))
	{
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	errorText = "";
	if (!this->mCompileShaderFromSource(vertexShad, errorText))
	{
		this->mLastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...



	fragShader.id = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;
	if (!this->mLoadSourceFromFile(fragShader))
	{
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	if (!this->mCompileShaderFromSource(fragShader, errorText))
	{
		this->mLastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...


	cShaderProgram curProgram;
	curProgram.id = glCreateProgram();

	glAttachShader(curProgram.id, vertexShad.id);
	glAttachShader(curProgram.id, fragShader.id);
	glLinkProgram(curProgram.id);

	// Was there a link error? 
	errorText = "";
	if (this->mWasThereALinkError(curProgram.id, errorText))
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->mLastError = ssError.str();
		return false;
	}

	// At this point, shaders are compiled and linked into a program

	curProgram.friendlyName = friendlyName;

	// Add the shader to the map
	this->mIdToShaderProgram[curProgram.id] = curProgram;
	// Save to other map, too
	this->mFriendlyNameToId[curProgram.friendlyName] = curProgram.id;

	return true;
}