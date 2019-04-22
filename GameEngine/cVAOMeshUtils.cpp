#include "cVAOMeshUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <iostream> 
#include <sstream>

#include "json.hpp"
#include "cJsonUtils.h"
#include "cShaderUtils.h"
#include "TextureManager/cBasicTextureManager.h"

#include "CPlyFile5nt.h"
#include "CStringHelper.h"

bool cVAOMeshUtils::loadFromSaveFile = false;

cVAOMeshUtils::cVAOMeshUtils()
{
	this->m_fileLoader = cVAOMeshUtils::PLY5N;
	this->m_basePath = "";
	return;
}


cVAOMeshUtils::~cVAOMeshUtils()
{
	return;
}

void cVAOMeshUtils::SetBasePath(std::string basepath)
{
	this->m_basePath = basepath;
	return;
}

cVAOMeshUtils* cVAOMeshUtils::pVAOMeshUtils = 0;

cVAOMeshUtils* cVAOMeshUtils::getInstance(void)
{
	if (pVAOMeshUtils == 0)
	{
		pVAOMeshUtils = new cVAOMeshUtils();
	}
	return pVAOMeshUtils;
}

unsigned int cVAOMeshUtils::m_roundUp(unsigned int numToRound, unsigned int multiple)
{
	if (multiple == 0)
	{
		return numToRound;
	}

	int remainder = numToRound % multiple;
	if (remainder == 0)
	{
		return numToRound;
	}

	return numToRound + multiple - remainder;
}

void cVAOMeshUtils::changeLoaderToPly5n(void)
{
	this->m_fileLoader = cVAOMeshUtils::PLY5N;
	return;
}

void cVAOMeshUtils::changeLoaderToAssimp(void)
{
	this->m_fileLoader = cVAOMeshUtils::ASSIMP;
	return;
}

bool cVAOMeshUtils::loadModels(GLuint program)
{
	changeLoaderToPly5n();
	std::vector<std::string> meshFileNames;
	std::vector<std::string> textures;
	std::vector<std::string> texturesHigh;
	if (loadFromSaveFile)
	{
		std::ifstream ifs("savefile.json");
		nlohmann::json j = json::parse(ifs);
		ifs.close();
		meshFileNames = j["meshFileNames"].get<std::vector<std::string>>();
		textures = j["textures"].get<std::vector<std::string>>();
		texturesHigh = j["texturesHigh"].get<std::vector<std::string>>();
	}
	else
	{
		meshFileNames = cJsonUtils::getJsonInstance()["meshFileNames"].get<std::vector<std::string>>();
		textures = cJsonUtils::getJsonInstance()["textures"].get<std::vector<std::string>>();
		texturesHigh = cJsonUtils::getJsonInstance()["texturesHigh"].get<std::vector<std::string>>();
	}

	for (size_t i = 0; i < meshFileNames.size(); i++)
	{
		cModelDrawInfo modelInfo;
		modelInfo.meshFileName = meshFileNames[i];
		if (!loadModelIntoVAO(modelInfo, program))
		{
			std::cout << "Didn't load the " << modelInfo.meshFileName << std::endl;
			std::cout << getLastError() << std::endl;
			return false;
		}
	}

	cBasicTextureManager::sCubeMapTextureLoadParams cubeLoadParamsSpace;
	cubeLoadParamsSpace.cubeMapName = "SpaceCubeMap";
	cubeLoadParamsSpace.posX_fileName = "SpaceBox_right1_posX.bmp";
	cubeLoadParamsSpace.negX_fileName = "SpaceBox_left2_negX.bmp";
	cubeLoadParamsSpace.posY_fileName = "SpaceBox_top3_posY.bmp";
	cubeLoadParamsSpace.negY_fileName = "SpaceBox_bottom4_negY.bmp";
	cubeLoadParamsSpace.posZ_fileName = "SpaceBox_front5_posZ.bmp";
	cubeLoadParamsSpace.negZ_fileName = "SpaceBox_back6_negZ.bmp";
	cubeLoadParamsSpace.basePath = "assets/textures/cubemaps";
	cubeLoadParamsSpace.bIsSeamless = true;

	cBasicTextureManager::sCubeMapTextureLoadParams cubeLoadParamsDay;
	cubeLoadParamsDay.cubeMapName = "CityCubeMap";
	cubeLoadParamsDay.posX_fileName = "TropicalSunnyDayRight2048.bmp";
	cubeLoadParamsDay.negX_fileName = "TropicalSunnyDayLeft2048.bmp";
	cubeLoadParamsDay.posY_fileName = "TropicalSunnyDayUp2048.bmp";
	cubeLoadParamsDay.negY_fileName = "TropicalSunnyDayDown2048.bmp";
	cubeLoadParamsDay.posZ_fileName = "TropicalSunnyDayFront2048.bmp";
	cubeLoadParamsDay.negZ_fileName = "TropicalSunnyDayBack2048.bmp";
	cubeLoadParamsDay.basePath = "assets/textures/cubemaps";
	cubeLoadParamsDay.bIsSeamless = true;

	std::string errorString;
	if (cBasicTextureManager::getInstance()->CreateCubeTextureFromBMPFiles(cubeLoadParamsDay, errorString))
	{
		std::cout << "Loaded the day cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: space cube map DIDN't load. On no!" << std::endl;
	}

	if (cBasicTextureManager::getInstance()->CreateCubeTextureFromBMPFiles(cubeLoadParamsSpace, errorString))
	{
		std::cout << "Loaded the space cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: space cube map DIDN't load. On no!" << std::endl;
	}

	for (size_t i = 0; i < textures.size(); i++)
	{
		cBasicTextureManager::getInstance()->Create2DTextureFromBMPFile(cBasicTextureManager::s2DTextureLoadParams(textures[i], "assets/textures"), true);
	}

	for (size_t i = 0; i < texturesHigh.size(); i++)
	{
		cBasicTextureManager::getInstance()->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams(texturesHigh[i], "assets/textures"), true);
	}



	return true;
}

bool cVAOMeshUtils::loadModelIntoVAO(
	cModelDrawInfo &drawInfo,
	unsigned int shaderProgramID)
{
	// See if this model has already been loaded
	std::map< std::string /*model name*/, cModelDrawInfo >::iterator itModel = this->mMapModelNameToDrawInfo.find(drawInfo.meshFileName);

	if (itModel != this->mMapModelNameToDrawInfo.end())
	{
		this->mAppendTextToLastError("Model ", false);
		this->mAppendTextToLastError(drawInfo.meshFileName, false);
		this->mAppendTextToLastError(" was already loaded.");
		return false;
	}



	// Load the model from the file
	if (drawInfo.numberOfVertices == 0)
	{
		if (!this->mLoadModelFromFile(drawInfo))
		{
			std::cout << "Didn't load " << drawInfo.meshFileName << " file." << std::endl;
			// Add some error code
			return false;
		}
	}

	// Load the model data into the GPU
	//LoadMeshIntoGPUBuffer
	if (!this->mLoadDrawInfoIntoVAO(drawInfo, shaderProgramID))
	{
		// Add some error text
		return false;
	}

	// Store this mesh into the map
	this->mMapModelNameToDrawInfo[drawInfo.meshFileName] = drawInfo;

	return true;
}

// Looks up draw info by name (meshFileName)			
bool cVAOMeshUtils::findDrawInfoByModelName(cModelDrawInfo &drawInfo)
{
	std::map< std::string /*model name*/, cModelDrawInfo >::iterator itModel = this->mMapModelNameToDrawInfo.find(drawInfo.meshFileName);

	if (itModel == this->mMapModelNameToDrawInfo.end())
	{
		// Didn't find model
		return false;
	}

	// Copy the draw info back to the caller.
	drawInfo = itModel->second;

	return true;
}

bool cVAOMeshUtils::m_LoadModelFromFile_AssimpLoader(cModelDrawInfo &drawInfo)
{
	assert(true);
	return false;
}

bool cVAOMeshUtils::mLoadModelFromFile(cModelDrawInfo &drawInfo)
{
	switch (this->m_fileLoader)
	{
		//case cVAOMeshManager::ORIGINAL:
		//	return this->m_LoadModelFromFile_OriginalLoader( drawInfo );
		//	break;
	case cVAOMeshUtils::PLY5N:
		return this->m_LoadModelFromFile_Ply5nLoader(drawInfo);
		break;
	case cVAOMeshUtils::ASSIMP:
		return this->m_LoadModelFromFile_AssimpLoader(drawInfo);
		break;
	}

	return false;
}

bool cVAOMeshUtils::m_LoadModelFromFile_Ply5nLoader(cModelDrawInfo &drawInfo)
{
	// Open the file that you asked.
	std::string fileToLoadFullPath = drawInfo.meshFileName;
	if (this->m_basePath != "")
	{
		fileToLoadFullPath = this->m_basePath + "/" + drawInfo.meshFileName;
	}
	std::ifstream theFile(drawInfo.meshFileName.c_str());

	CPlyFile5nt plyLoader;
	std::wstring error;
	if (!plyLoader.OpenPLYFile2(CStringHelper::ASCIIToUnicodeQnD(fileToLoadFullPath), error))
	{
		this->mAppendTextToLastError("Didn't load the ", false);
		this->mAppendTextToLastError(fileToLoadFullPath, false);
		this->mAppendTextToLastError(" file.");
		this->mAppendTextToLastError(CStringHelper::UnicodeToASCII_QnD(error));
		return false;
	}

	drawInfo.pMeshData = new cMesh();

	drawInfo.numberOfVertices = plyLoader.GetNumberOfVerticies();
	drawInfo.pMeshData->numberOfVertices = plyLoader.GetNumberOfVerticies();
	//	std::cout << "vertices: " << drawInfo.numberOfVertices << std::endl;

	drawInfo.numberOfTriangles = plyLoader.GetNumberOfElements();
	drawInfo.pMeshData->numberOfTriangles = plyLoader.GetNumberOfElements();

	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
	drawInfo.pMeshData->numberOfIndices = drawInfo.numberOfIndices;

	drawInfo.pMeshData->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[this->m_roundUp(drawInfo.numberOfVertices, 64)];
	drawInfo.pMeshData->pTriangles = new cTriangle[this->m_roundUp(drawInfo.numberOfTriangles, 64)];
	drawInfo.pMeshData->pIndices = new unsigned int[this->m_roundUp(drawInfo.numberOfIndices, 64)];

	PlyVertex curVert;
	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	{
		curVert = plyLoader.getVertex_at(index);

		drawInfo.pMeshData->pVertices[index].x = curVert.xyz.x;
		drawInfo.pMeshData->pVertices[index].y = curVert.xyz.y;
		drawInfo.pMeshData->pVertices[index].z = curVert.xyz.z;

		drawInfo.pMeshData->pVertices[index].nx = curVert.nx;
		drawInfo.pMeshData->pVertices[index].ny = curVert.ny;
		drawInfo.pMeshData->pVertices[index].nz = curVert.nz;

		//  Also load the UV values
		drawInfo.pMeshData->pVertices[index].u0 = curVert.tex0u;
		drawInfo.pMeshData->pVertices[index].v0 = curVert.tex0v;
		drawInfo.pMeshData->pVertices[index].u1 = 0.0f;
		drawInfo.pMeshData->pVertices[index].v1 = 0.0f;

		drawInfo.pMeshData->pVertices[index].r = curVert.red;
		drawInfo.pMeshData->pVertices[index].g = curVert.green;
		drawInfo.pMeshData->pVertices[index].b = curVert.blue;
		drawInfo.pMeshData->pVertices[index].a = curVert.alpha;

		drawInfo.pMeshData->pVertices[index].bx = 0.0f;
		drawInfo.pMeshData->pVertices[index].by = 0.0f;
		drawInfo.pMeshData->pVertices[index].bz = 0.0f;

		drawInfo.pMeshData->pVertices[index].tx = 0.0f;
		drawInfo.pMeshData->pVertices[index].ty = 0.0f;
		drawInfo.pMeshData->pVertices[index].tz = 0.0f;

		for (unsigned int boneIndex = 0; boneIndex != NUMBEROFBONES; boneIndex++)
		{
			drawInfo.pMeshData->pVertices[index].boneID[boneIndex] = 0;
			drawInfo.pMeshData->pVertices[index].boneWeights[boneIndex] = 0.0f;
		}

		//		theFile >> g_pArrayVert[index].;
	}//for ( unsigned int index...


	PlyElement curElement;
	unsigned int index = 0;
	unsigned int triIndex = 0;
	for (; triIndex != drawInfo.numberOfTriangles;
		index += 3, triIndex++)
	{
		curElement = plyLoader.getElement_at(triIndex);

		drawInfo.pMeshData->pTriangles[triIndex].vertex_ID_0 = curElement.vertex_index_1;
		drawInfo.pMeshData->pTriangles[triIndex].vertex_ID_1 = curElement.vertex_index_2;
		drawInfo.pMeshData->pTriangles[triIndex].vertex_ID_2 = curElement.vertex_index_3;

		// Unpack the index information, too
		drawInfo.pMeshData->pIndices[index + 0] = curElement.vertex_index_1;
		drawInfo.pMeshData->pIndices[index + 1] = curElement.vertex_index_2;
		drawInfo.pMeshData->pIndices[index + 2] = curElement.vertex_index_3;

	}//for ( unsigned int index...

	plyLoader.calcualteExtents();

	drawInfo.min.x = plyLoader.getMinX();		drawInfo.pMeshData->minXYZ.x = plyLoader.getMinX();
	drawInfo.min.y = plyLoader.getMinY();		drawInfo.pMeshData->minXYZ.y = plyLoader.getMinY();
	drawInfo.min.z = plyLoader.getMinZ();		drawInfo.pMeshData->minXYZ.z = plyLoader.getMinZ();

	drawInfo.max.x = plyLoader.getMaxX();		drawInfo.pMeshData->maxXYZ.x = plyLoader.getMaxX();
	drawInfo.max.y = plyLoader.getMaxY();		drawInfo.pMeshData->maxXYZ.y = plyLoader.getMaxY();
	drawInfo.max.z = plyLoader.getMaxZ();		drawInfo.pMeshData->maxXYZ.z = plyLoader.getMaxZ();

	drawInfo.extent.x = drawInfo.max.x - drawInfo.min.x;		drawInfo.pMeshData->maxExtentXYZ.x = drawInfo.extent.x;
	drawInfo.extent.y = drawInfo.max.y - drawInfo.min.y;		drawInfo.pMeshData->maxExtentXYZ.y = drawInfo.extent.y;
	drawInfo.extent.z = drawInfo.max.z - drawInfo.min.z;		drawInfo.pMeshData->maxExtentXYZ.z = drawInfo.extent.z;

	drawInfo.maxExtent = plyLoader.getMaxExtent();		drawInfo.pMeshData->maxExtent = drawInfo.maxExtent;


	return true;

	/******************old code********************/
	// if ( theFile.is_open() == false )
	//if (!theFile.is_open())			// More "c" or "C++" ish
	//{
	//	std::cout << "Didn't open file" << std::endl;
	//	this->mAppendTextToLastError("Didn't load the ", false);
	//	this->mAppendTextToLastError(drawInfo.meshFileName, false);
	//	this->mAppendTextToLastError(" file.");
	//	return false;
	//}//if (!theFile.is_open() 

	// file is open OK
//	std::string nextData;
//	while (theFile >> nextData)
//	{
//		if (nextData == "vertex")
//		{
//			break;		// exit while loop...
//		}
//	};
//	// ...Jumping down to here
//
////	unsigned int numberOfVertices = 0;
////	theFile >> g_numberOfVertices;
//	theFile >> drawInfo.numberOfVertices;
//
//		//std::cout << "vertices: " << g_numberOfVertices << std::endl;
//	std::cout << "vertices: " << drawInfo.numberOfVertices << std::endl;
//
//	// seach for "face"
//	while (theFile >> nextData)
//	{
//		if (nextData == "face")
//		{
//			break;		// exit while loop...
//		}
//	};
//	// ...Jumping down to here
//
////	unsigned int numberOfTriangles = 0;
////	theFile >> g_numberOfTriangles;
//	theFile >> drawInfo.numberOfTriangles;
//
//	//	std::cout << "triangles: " << g_numberOfTriangles << std::endl;
//	std::cout << "triangles: " << drawInfo.numberOfTriangles << std::endl;
//
//	while (theFile >> nextData)
//	{
//		if (nextData == "end_header")
//		{
//			break;		// exit while loop...
//		}
//	};
//	// ...Jumping down to here
//
//	//-0.036872 0.127727 0.00440925 
//	//-0.0453607 0.128854 0.00114541 
//
//	// Create an vertex array to store the data.
////	sPlyVertex tempVert; 
////	sPlyVertex tempArrayVert[1000];		// Static (stack)
//
////	sPlyVertex* pArrayVert = new sPlyVertex[numberOfVertices];	// HEAP
////	g_pArrayVert = new sPlyVertex[g_numberOfVertices];	// HEAP
//	drawInfo.pVerticesFromFile = new glm::vec3[drawInfo.numberOfVertices];
//	drawInfo.pNormalsFromFile = new glm::vec3[drawInfo.numberOfVertices];
//	drawInfo.pTextureCoordsFromFile = new glm::vec2[drawInfo.numberOfVertices];
//	//	ZeroMemory(); win32
//		// C call... (clears memory to all zeros)
//	//	memset( g_pArrayVert, 0, sizeof( sPlyVertex ) * g_numberOfVertices );
//	memset(drawInfo.pVerticesFromFile, 0, sizeof(glm::vec3) * drawInfo.numberOfVertices);
//
//	// Read the vertex data into the array
//	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
//	{
//		theFile >> drawInfo.pVerticesFromFile[index].x;
//		theFile >> drawInfo.pVerticesFromFile[index].y;
//		theFile >> drawInfo.pVerticesFromFile[index].z;
//
//		theFile >> drawInfo.pNormalsFromFile[index].x;
//		theFile >> drawInfo.pNormalsFromFile[index].y;
//		theFile >> drawInfo.pNormalsFromFile[index].z;
//
//		theFile >> drawInfo.pTextureCoordsFromFile[index].x;
//		theFile >> drawInfo.pTextureCoordsFromFile[index].y;
//
//		//		theFile >> g_pArrayVert[index].;
//	}//for ( unsigned int index...
//
//	// Same with triangles
//
////	sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles];	// HEAP
////	g_pArrayTris = new sPlyTriangle[g_numberOfTriangles];	// HEAP
//	drawInfo.pTriangles = new glm::ivec3[drawInfo.numberOfTriangles];
//
//	//	memset( g_pArrayTris, 0, sizeof( sPlyTriangle ) * g_numberOfTriangles );
//	memset(drawInfo.pTriangles, 0, sizeof(glm::vec3) * drawInfo.numberOfTriangles);
//
//	int TossThisAway = 0;
//	for (unsigned int index = 0; index != drawInfo.numberOfTriangles; index++)
//	{
//		// 3 69 1322 70
//		theFile >> TossThisAway;			// 3
//		theFile >> drawInfo.pTriangles[index].x;
//		theFile >> drawInfo.pTriangles[index].y;
//		theFile >> drawInfo.pTriangles[index].z;
//	}//for ( unsigned int index...
//
//	std::cout << "Read from the file OK." << std::endl;
//
//	// Calculating extents...
//
//	// Assume the 1st one is the largest and smallest:
//	drawInfo.min = drawInfo.pVerticesFromFile[0];
//
//	drawInfo.max = drawInfo.pVerticesFromFile[0];
//
//	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
//	{
//		if (drawInfo.pVerticesFromFile[index].x < drawInfo.min.x) { drawInfo.min.x = drawInfo.pVerticesFromFile[index].x; }
//		if (drawInfo.pVerticesFromFile[index].y < drawInfo.min.y) { drawInfo.min.y = drawInfo.pVerticesFromFile[index].y; }
//		if (drawInfo.pVerticesFromFile[index].z < drawInfo.min.z) { drawInfo.min.z = drawInfo.pVerticesFromFile[index].z; }
//
//		if (drawInfo.pVerticesFromFile[index].x > drawInfo.max.x) { drawInfo.max.x = drawInfo.pVerticesFromFile[index].x; }
//		if (drawInfo.pVerticesFromFile[index].y > drawInfo.max.y) { drawInfo.max.y = drawInfo.pVerticesFromFile[index].y; }
//		if (drawInfo.pVerticesFromFile[index].z > drawInfo.max.z) { drawInfo.max.z = drawInfo.pVerticesFromFile[index].z; }
//	}//for ( unsigned int index...
//
//	drawInfo.extent.x = drawInfo.max.x - drawInfo.min.x;
//	drawInfo.extent.y = drawInfo.max.y - drawInfo.min.y;
//	drawInfo.extent.z = drawInfo.max.z - drawInfo.min.z;
//
//	drawInfo.maxExtent = drawInfo.extent.x;
//	if (drawInfo.maxExtent < drawInfo.extent.y) { drawInfo.maxExtent = drawInfo.extent.y; }
//	if (drawInfo.maxExtent < drawInfo.extent.z) { drawInfo.maxExtent = drawInfo.extent.z; }
//
//	return true;
}

bool cVAOMeshUtils::mLoadDrawInfoIntoVAO(
	cModelDrawInfo &drawInfo,
	unsigned int shaderProgramID)
{

	// See if we've been passed a cMesh object...
	if (drawInfo.pMeshData == NULL)
	{
		this->mAppendTextToLastError("cMesh object is missing (NULL). Can't load mesh into GPU buffers.");
		return false;
	}
	if (drawInfo.pMeshData->numberOfVertices == 0)
	{
		this->mAppendTextToLastError("There are no vertices in the cMesh object. Can't load anything into GPU.");
		return false;
	}
	if (drawInfo.pMeshData->pVertices == NULL)
	{
		this->mAppendTextToLastError("cMesh object is NULL. Nothing to load into GPU.");
		return false;
	}

	// Creates a VAO (Vertex Array Object)
	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	glBindVertexArray(drawInfo.VAO_ID);

	// Allocate a buffer and copy
	//GLuint vertex_buffer;

	// Just to confuse you, this is called VBO (Vertex Buffer Object)
	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &(drawInfo.vertexBufferID));			// vertex_buffer
	// "Vertex" buffer
	// - Sets the "type" of buffer
	// - Makes it the 'current' buffer
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int vertexBufferSizeInBytes =
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * drawInfo.numberOfVertices;

	//	vector<sVert_xyz_rgb> vecVerticesON_THE_CPU;
	//	sVert_xyz_rgb* pVerticesToVBO = new sVert_xyz_rgb[ARRAYSIZE]

	if (drawInfo.bVertexBufferIsDynamic)
	{
		// CAN update the vertex information (later)
		glBufferData(GL_ARRAY_BUFFER,
			vertexBufferSizeInBytes,		// sizeof(vertices), 
			drawInfo.pMeshData->pVertices,			// vertices, 
			GL_DYNAMIC_DRAW);	//*************
	}
	else
	{
		// DON'T INDEND to update the vertex buffer info (faster)
		glBufferData(GL_ARRAY_BUFFER,
			vertexBufferSizeInBytes,	// sizeof(vertices), 
			drawInfo.pMeshData->pVertices,			// vertices, 
			GL_STATIC_DRAW);	//*************
	}

	// Create the index buffer...


	glGenBuffers(1, &(drawInfo.indexBufferID));			// vertex_buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.indexBufferID);

	unsigned int indexBufferSizeInBytes = sizeof(unsigned int) * drawInfo.numberOfIndices;


	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferSizeInBytes,	// sizeof(vertices), 
		drawInfo.pMeshData->pIndices,			// vertices, 
		GL_STATIC_DRAW);

	// Set the vertex layout

	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");
	GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");
	GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUV_x2");
	// Add the remaining vertex attributes (binormal, tangent, bone ID and weight)
	GLint vpos_vTanXYZ_Location = glGetAttribLocation(shaderProgramID, "vTanXYZ");
	GLint vpos_vBiNormXYZ_Location = glGetAttribLocation(shaderProgramID, "vBiNormXYZ");
	GLint vpos_vBoneID_Location = glGetAttribLocation(shaderProgramID, "vBoneID");
	GLint vpos_vBoneWeight_Location = glGetAttribLocation(shaderProgramID, "vBoneWeight");



	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location,	// "vPosition"
		4,				//  vec3 (xyz)
		GL_FLOAT,
		GL_FALSE,		// DON'T "normalize"
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, x));
	//						   (void*) 0);

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location,		// cColour
		4,					// vec3 (rgb)
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, r));
	//						   (void*) (sizeof(float) * 3));

		// Now add the normal vertex attribute
	glEnableVertexAttribArray(vnorm_location);
	glVertexAttribPointer(vnorm_location,		// 
		4,					// vec3 (xyz)
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	//   9  size in bytes
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, nx));

	// Now add the normal vertex attribute
	glEnableVertexAttribArray(vUVx2_location);
	glVertexAttribPointer(vUVx2_location,		// 
		4,					// vec4 vUV_x2;
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	//   float u0, v0, u1, v1;
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, u0));

	// Add the other 4 things we now have in the Skinned Mesh vertex layout
	//glEnableVertexAttribArray(vpos_vTanXYZ_Location);
	//glVertexAttribPointer(vpos_vTanXYZ_Location  ,		// vTanXYZ
				//		   4,					// vec4 
				//		   GL_FLOAT, 
				//		   GL_FALSE,
	//                       sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
				//		   (void*) offsetof( sVertex_xyz_rgba_n_uv2_bt_4Bones, tx ) );

	//glEnableVertexAttribArray(vpos_vBiNormXYZ_Location );
	//glVertexAttribPointer(vpos_vBiNormXYZ_Location   ,		// vBiNormXYZ
				//		   4,								// vec4 
				//		   GL_FLOAT, 
				//		   GL_FALSE,
	//                       sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
				//		   (void*) offsetof( sVertex_xyz_rgba_n_uv2_bt_4Bones, bx ) );

	glEnableVertexAttribArray(vpos_vBoneID_Location);
	glVertexAttribPointer(vpos_vBoneID_Location,		// vBoneID
		4,								// vec4 
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, boneID[0]));

	glEnableVertexAttribArray(vpos_vBoneWeight_Location);
	glVertexAttribPointer(vpos_vBoneWeight_Location,		// vBoneID
		4,								// vec4 
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, boneWeights[0]));


	// Get rid of everything we don't need.

	// Set the "current" VAO to nothing.
	glBindVertexArray(0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vnorm_location);
	glDisableVertexAttribArray(vUVx2_location);
	// The added skinned mesh elements of the vertex class
	glDisableVertexAttribArray(vpos_vTanXYZ_Location);
	glDisableVertexAttribArray(vpos_vBiNormXYZ_Location);
	glDisableVertexAttribArray(vpos_vBoneID_Location);
	glDisableVertexAttribArray(vpos_vBoneWeight_Location);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return true;

	/********old code*******/
	//sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_numberOfVertices];
	//drawInfo.pVerticesToVBO = new sVert_xyz_rgb_n_uv2[drawInfo.numberOfVertices];

	//// Copy the data from the PLY format to the vertex buffer format
	//for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	//{
	//	drawInfo.pVerticesToVBO[index].xyz = drawInfo.pVerticesFromFile[index];

	//	glm::vec3 normal = drawInfo.pNormalsFromFile[index];
	//	glm::normalize(normal);

	//	drawInfo.pVerticesToVBO[index].n = normal;

	//	// Set all the colours to white (1,1,1) for now
	//	drawInfo.pVerticesToVBO[index].rgb = glm::vec3(1.0f);

	//	// Set the UV values, also
	//	drawInfo.pVerticesToVBO[index].uv0 = drawInfo.pTextureCoordsFromFile[index];
	//	// There might be a 2nd set of UV coordinates, but not now
	//	drawInfo.pVerticesToVBO[index].uv1 = glm::vec2(0.0f);
	//}


	//// Creates a VAO (Vertex Array Object)
	//glGenVertexArrays(1, &(drawInfo.VAO_ID));
	//glBindVertexArray(drawInfo.VAO_ID);


	//// Allocate a buffer and copy
	////GLuint vertex_buffer;

	//// Just to confuse you, this is called VBO (Vertex Buffer Object)
	//// NOTE: OpenGL error checks have been omitted for brevity
	//glGenBuffers(1, &(drawInfo.vertexBufferID));			// vertex_buffer
	//// "Vertex" buffer
	//// - Sets the "type" of buffer
	//// - Makes it the 'current' buffer
	//glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	//unsigned int vertexBufferSizeInBytes =
	//	sizeof(sVert_xyz_rgb_n_uv2) * drawInfo.numberOfVertices;

	////	vector<sVert_xyz_rgb> vecVerticesON_THE_CPU;
	////	sVert_xyz_rgb* pVerticesToVBO = new sVert_xyz_rgb[ARRAYSIZE]

	//if (drawInfo.bVertexBufferIsDynamic)
	//{
	//	// CAN update the vertex information (later)
	//	glBufferData(GL_ARRAY_BUFFER,
	//		vertexBufferSizeInBytes,		// sizeof(vertices), 
	//		drawInfo.pVerticesToVBO,			// vertices, 
	//		GL_DYNAMIC_DRAW);	//*************
	//}
	//else
	//{
	//	// DON'T INDEND to update the vertex buffer info (faster)
	//	glBufferData(GL_ARRAY_BUFFER,
	//		vertexBufferSizeInBytes,	// sizeof(vertices), 
	//		drawInfo.pVerticesToVBO,			// vertices, 
	//		GL_STATIC_DRAW);	//*************
	//}

	//// Create the index buffer...

	//// Three indices (of vertices) per triangle
	//drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
	//drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];
	//memset(drawInfo.pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices);


	//// Load the index buffer...
	//unsigned int indexIndex = 0;
	//for (unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles;
	//	triIndex++, indexIndex += 3)
	//{
	//	drawInfo.pIndices[indexIndex + 0] = drawInfo.pTriangles[triIndex].x;
	//	drawInfo.pIndices[indexIndex + 1] = drawInfo.pTriangles[triIndex].y;
	//	drawInfo.pIndices[indexIndex + 2] = drawInfo.pTriangles[triIndex].z;
	//}


	//glGenBuffers(1, &(drawInfo.indexBufferID));			// vertex_buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.indexBufferID);

	//unsigned int indexBufferSizeInBytes = sizeof(unsigned int) * drawInfo.numberOfIndices;

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//	indexBufferSizeInBytes,	// sizeof(vertices), 
	//	drawInfo.pIndices,			// vertices, 
	//	GL_STATIC_DRAW);

	//// Set the vertex layout

	//GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");
	//GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");
	//GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");
	//GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUV_x2");

	//glEnableVertexAttribArray(vpos_location);
	//glVertexAttribPointer(vpos_location,	// "vPosition"
	//	3,				//  vec3 (xyz)
	//	GL_FLOAT,
	//	GL_FALSE,		// DON'T "normalize"
	//	sizeof(sVert_xyz_rgb_n_uv2),
	//	(void*) offsetof(sVert_xyz_rgb_n_uv2, xyz));
	//	//sizeof(float) * 6,
	//	//(void*)0);

	//glEnableVertexAttribArray(vcol_location);
	//glVertexAttribPointer(vcol_location,		// cColour
	//	3,					// vec3 (rgb)
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(sVert_xyz_rgb_n_uv2), //sizeof(float) * 6,
	//	(void*) offsetof(sVert_xyz_rgb_n_uv2, rgb));//(void*)(sizeof(float) * 3));

	//glEnableVertexAttribArray(vnorm_location);
	//glVertexAttribPointer(vnorm_location,		// 
	//	3,					// vec3 (xyz)
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(sVert_xyz_rgb_n_uv2),	//   9  size in bytes
	//	(void*) offsetof(sVert_xyz_rgb_n_uv2, n));

	//// Now add the texture vertex attribute
	//glEnableVertexAttribArray(vUVx2_location);
	//glVertexAttribPointer(vUVx2_location,		// 
	//	4,					// vec4 vUV_x2;
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(sVert_xyz_rgb_n_uv2),	//   float u0, v0, u1, v1;
	//	(void*)offsetof(sVert_xyz_rgb_n_uv2, uv0));

	//// Get rid of everything we don't need.

	//// Set the "current" VAO to nothing.
	//glBindVertexArray(0);

	//glDisableVertexAttribArray(vpos_location);
	//glDisableVertexAttribArray(vcol_location);
	//glDisableVertexAttribArray(vnorm_location);
	//glDisableVertexAttribArray(vUVx2_location);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//return true;
}

void cVAOMeshUtils::mAppendTextToLastError(std::string text, bool addNewLineBefore /*=true*/)
{
	std::stringstream ssError;
	ssError.str() = this->mLastErrorString;

	if (addNewLineBefore)
	{
		ssError << std::endl;
	}
	ssError << text;

	this->mLastErrorString = ssError.str();

	return;
}

std::string cVAOMeshUtils::getLastError(bool bAndClear /*=true*/)
{
	std::string errorText = this->mLastErrorString;
	if (bAndClear)
	{
		this->mLastErrorString = "";
	}
	return errorText;
}

void cVAOMeshUtils::shutDown(void)
{
	//clear the map
	return;
}

void cVAOMeshUtils::updateModelVertexInformation(cModelDrawInfo &drawInfo)
{
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int vertexBufferSizeInBytes =
		sizeof(sVert_xyz_rgb_n_uv2) * drawInfo.numberOfVertices;

	// ASSUME it's a dynmaic buffer
//	if ( drawInfo.bVertexBufferIsDynamic )
//	{	
		// CAN update the vertex information (later)
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSizeInBytes,		// sizeof(vertices), 
		drawInfo.pMeshData->pVertices,			// vertices, 
		GL_DYNAMIC_DRAW);	//*************
//	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);	// "un-bind" the buffer

	return;
}
