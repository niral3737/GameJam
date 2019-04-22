#ifndef _MODEL_DRAW_INFO_HG_
#define _MODEL_DRAW_INFO_HG_

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "cMesh.h"

struct sVert_xyz_rgb_n_uv2
{
	glm::vec3 xyz {0.0f ,0.0f ,0.0f};
	glm::vec3 rgb { 0.0f ,0.0f ,0.0f };
	glm::vec3 n { 0.0f, 0.0f, 0.0f };
	glm::vec2 uv0{ 0.0f, 0.0f };
	glm::vec2 uv1{ 0.0f, 0.0f };
};

class cModelDrawInfo
{
public:
	cModelDrawInfo();
	~cModelDrawInfo();

	std::string meshFileName;
	std::string friendlyName;	// The one we want to use for actual drawing

	unsigned int VAO_ID;					

	unsigned int vertexBufferID;
	unsigned int vertexBuffer_start_index;
	unsigned int numberOfVertices;

	unsigned int indexBufferID;
	unsigned int indexBuffer_start_index;
	unsigned int numberOfIndices;			
	unsigned int numberOfTriangles;

	// This is the vertex information as read from the file
	//glm::vec3* pVerticesFromFile;
	//glm::vec3* pNormalsFromFile;
	//glm::vec2* pTextureCoordsFromFile;
	//// These are the original triangle values from file,
	////	but in the 'triangle' (or face) format
	//glm::ivec3 *pTriangles;

	//// This is the vertex information being passed to the GPU
	//// (is in format that the shader needs)
	//sVert_xyz_rgb_n_uv2* pVerticesToVBO;

	//// This buffer is indices as a 1D array (that the GPU needs)
	//unsigned int* pIndices;		// = new unsigned int[ARRAYSIZE]

	glm::vec3 max;
	glm::vec3 min;
	glm::vec3 extent;
	float maxExtent;


	// ******************************************
	// This is to draw "debug lines" in the scene
	// HACK: Can make this dynamic
	bool bVertexBufferIsDynamic;	// = false by default
	bool bIsIndexedModel;			// = true by default

	cMesh* pMeshData;
};

#endif // !_MODEL_DRAW_INFO_HG_



