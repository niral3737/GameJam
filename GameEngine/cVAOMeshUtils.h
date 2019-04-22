#ifndef _VAO_MESH_UTILS_HG_
#define _VAO_MESH_UTILS_HG_

#include <glad/glad.h>
#include <string>
#include <map>
#include "cModelDrawInfo.h"

#include "cAnimationState.h"

class cVAOMeshUtils
{
public:
	~cVAOMeshUtils();

	enum eLoadWith
	{
		/*ORIGINAL,*/
		PLY5N,
		ASSIMP
	};
	void changeLoaderToPly5n(void);
	void changeLoaderToAssimp(void);

	static cVAOMeshUtils* getInstance(void);
	static bool loadFromSaveFile;
	bool loadModels(GLuint program);
	// Note: the shader program ID is needed to tie 
	// the buffer to the vertex layout of the shader
	bool loadModelIntoVAO(cModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);

	// Looks up draw info by name (meshFileName)			
	bool findDrawInfoByModelName(cModelDrawInfo &drawInfo);

	void shutDown(void);

	std::string getLastError(bool bClear = true);

	// Update the mesh information, then re-copy to GPU
	// - This will RE-COPY the VERTEX infoarmation into the vertex buffer
	//   (will copy ENTIRE vertex buffer, OVERWRITING what's there)
	void updateModelVertexInformation(cModelDrawInfo &drawInfo);

	void SetBasePath(std::string basepath);

private:
	cVAOMeshUtils();
	static cVAOMeshUtils* pVAOMeshUtils;

	// Rounds up to multiples of 64
	unsigned int m_roundUp(unsigned int numToRound, unsigned int multiple);

	// LoadPlyFileData()
	bool mLoadModelFromFile(cModelDrawInfo &drawInfo);
	bool m_LoadModelFromFile_Ply5nLoader(cModelDrawInfo &drawInfo);
	bool m_LoadModelFromFile_AssimpLoader(cModelDrawInfo &drawInfo);

	//LoadMeshIntoGPUBuffer
	bool mLoadDrawInfoIntoVAO(cModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);


	// This holds the model information
	std::map< std::string /*model name*/, cModelDrawInfo > mMapModelNameToDrawInfo;

	std::string mLastErrorString;
	void mAppendTextToLastError(std::string text, bool addNewLineBefore = true);

	std::string m_basePath;

	eLoadWith m_fileLoader;
};

#endif // !_VAO_MESH_UTILS_HG_



