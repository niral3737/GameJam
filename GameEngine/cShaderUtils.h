#ifndef _SHADER_UTILS_HG_
#define _SHADER_UTILS_HG_
#include <glad/glad.h>

#include <string>
#include <vector>
#include <map>

class cShaderUtils
{
public:
	class cShader
	{
	public:
		cShader();
		~cShader();
		enum eShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			UNKNOWN
		} shaderType;

		unsigned int id;
		std::vector<std::string> vecSource;
		bool isSourceMultiline;
		std::string fileName;

		std::string getShaderTypeString(void);
	};

	class cShaderProgram
	{
	public:	
		cShaderProgram() : id(0) {};
		~cShaderProgram() {};

		unsigned int id;
		std::string friendlyName;
		//map will have uniform name as key and uniform location id as value
		std::map<std::string, int> mapUniformNameToUniformLocation;

		int getUniformIdFromName(std::string name);
		bool loadUniformLocation(std::string variableName);

	};
	
	~cShaderUtils();

	static GLuint setUpShaders();
	static cShaderUtils* getInstance(void);

	bool useShaderProgram(unsigned int id);
	bool useShaderProgram(std::string friendlyName);
	bool createProgramFromFile(std::string friendlyName, cShader &vertexShader, cShader &fragmentShader);
	void setBasePath(std::string basePath);
	unsigned int getIdFromFriendlyName(std::string friendlyName);
	cShaderProgram* getShaderProgramFromFriendlyName(std::string friendlyName);
	std::string getLastError(void);
	GLuint getUniformVariableLocation(GLuint program, std::string name);

private:
	cShaderUtils();
	static cShaderUtils* pShaderUtils;

	bool mLoadSourceFromFile(cShader &shader);
	bool mCompileShaderFromSource(cShader &shader, std::string &error);
	bool mWasThereACompileError(unsigned int shaderId, std::string &errorText);
	bool mWasThereALinkError(unsigned int programId, std::string &errorText);

	std::string mBasePath;
	std::string mLastError;
	std::map<unsigned int, cShaderProgram > mIdToShaderProgram;
	std::map<std::string, unsigned int> mFriendlyNameToId;
};

#endif // !_SHADER_UTILS_HG_



