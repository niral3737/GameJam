#include "cSceneUtils.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "cJsonUtils.h"
#include "cMeshObjectFactory.h"
#include "cModelDrawInfo.h"
#include "cVAOMeshUtils.h"
#include "cCamera.h"
#include "TextureManager/cBasicTextureManager.h"
#include <map>

cSceneUtils* cSceneUtils::pSceneUtils = 0;

glm::vec3 cSceneUtils::cameraEye = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cSceneUtils::cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);

bool cSceneUtils::loadFromSaveFile = false;


cSceneUtils::cSceneUtils()
{
	this->selectedMeshObject = NULL;
	this->selectedObjectIndex = -1;
	this->terrainHierarchy = new cAABBHierarchy();
	this->showAABBs = false;
	this->sceneCommandGroup.pause = true;
	this->dayMix = 1.0f;
	g_HACK_CurrentTime = 0.0f;
	this->playerForward = glm::vec3(0.0f, 0.0f, 1.0f);
	this->dalekLocation[0] = 1;
	this->dalekLocation[1] = 1;
	this->playerLocation[0] = 1;
	this->playerLocation[1] = 1;
	this->mDt = 0.0f;
	this->timePassed = 0.0f;
	walkAnimTime = 0.0f;
	this->nextPlayerPosition = glm::vec3(0.0f);
	this->shouldUpdate = false;
	this->selectedCharacter = NULL;
	this->selectedCharacterIndex = -1;
	this->blur = false;
	this->drawReticle = false;
	this->stencil = false;
	this->toon = false;
}

cSceneUtils::~cSceneUtils()
{}

cSceneUtils * cSceneUtils::getInstance(void)
{
	if (pSceneUtils == 0)
	{
		pSceneUtils = new cSceneUtils();
	}
	return pSceneUtils;
}

iMeshObject* cSceneUtils::findObjectByFriendlyName(std::string theNameToFind)
{
	for (unsigned int index = 0; index != vecObjectsToDraw.size(); index++)
	{
		cMeshObject* object = (cMeshObject*)vecObjectsToDraw[index];
		
		if (object->friendlyName == theNameToFind)
		{
			return vecObjectsToDraw[index];
		}
	}

	return NULL;	
}

void cSceneUtils::loadModelsIntoScene()
{
	//load models 
	std::vector<nlohmann::json> meshes;
	if (loadFromSaveFile)
	{
		std::ifstream ifs("savefile.json");
		nlohmann::json j = json::parse(ifs);
		ifs.close();
		meshes = j["meshes"].get<std::vector<nlohmann::json>>();
	}
	else
	{
		meshes = cJsonUtils::getJsonInstance()["meshes"].get<std::vector<nlohmann::json>>();
	}
	for (size_t i = 0; i < meshes.size(); i++)
	{
		cMeshObject* meshObject = (cMeshObject*) cMeshObjectFactory::createMeshObject();
		meshObject->meshName = meshes[i]["meshName"].get<std::string>();
		meshObject->friendlyName = meshes[i]["friendlyName"].get<std::string>();

		meshObject->isWireFrame = meshes[i]["isWireFrame"].get<bool>();
		meshObject->isVisible = meshes[i]["isVisible"].get<bool>();
		meshObject->useVertexColor = meshes[i]["useVertexColor"].get<bool>();
		meshObject->dontLight = meshes[i]["dontLight"].get<bool>();

		meshObject->position.x = meshes[i]["position"]["x"].get<float>();
		meshObject->position.y = meshes[i]["position"]["y"].get<float>();
		meshObject->position.z = meshes[i]["position"]["z"].get<float>();

		glm::vec3 eularAngles;
		eularAngles.x = meshes[i]["postRotation"]["x"].get<float>();
		eularAngles.y = meshes[i]["postRotation"]["y"].get<float>();
		eularAngles.z = meshes[i]["postRotation"]["z"].get<float>();

		meshObject->setOrientationEulerAngles(eularAngles, false);

		meshObject->materialDiffuse.r = meshes[i]["materialDiffuse"]["r"].get<float>();
		meshObject->materialDiffuse.g = meshes[i]["materialDiffuse"]["g"].get<float>();
		meshObject->materialDiffuse.b = meshes[i]["materialDiffuse"]["b"].get<float>();
		meshObject->materialDiffuse.a = meshes[i]["materialDiffuse"]["a"].get<float>();

		meshObject->materialSpecular.r = meshes[i]["materialSpecular"]["r"].get<float>();
		meshObject->materialSpecular.g = meshes[i]["materialSpecular"]["g"].get<float>();
		meshObject->materialSpecular.b = meshes[i]["materialSpecular"]["b"].get<float>();
		meshObject->materialSpecular.a = meshes[i]["materialSpecular"]["power"].get<float>();

		meshObject->scale = meshes[i]["scale"].get<float>();

		meshObject->isUpdatedByPhysics = meshes[i]["isUpdatedByPhysics"].get<bool>();

		meshObject->velocity.x = meshes[i]["velocity"]["x"].get<float>();
		meshObject->velocity.y = meshes[i]["velocity"]["y"].get<float>();
		meshObject->velocity.z = meshes[i]["velocity"]["z"].get<float>();

		meshObject->acceleration.x = meshes[i]["acceleration"]["x"].get<float>();
		meshObject->acceleration.y = meshes[i]["acceleration"]["y"].get<float>();
		meshObject->acceleration.z = meshes[i]["acceleration"]["z"].get<float>();

		if (meshes[i].find("textures") != meshes[i].end())
		{
			std::vector<nlohmann::json> textures = meshes[i]["textures"].get<std::vector<nlohmann::json>>();
			for (size_t j = 0; j < textures.size(); j++)
			{
				sTextureInfo texture;
				texture.name = textures[j]["name"].get<std::string>();
				texture.strength = textures[j]["strength"].get<float>();
				meshObject->vecTextures.push_back(texture);
			}
		}

		if (meshes[i].find("lodMeshes") != meshes[i].end())
		{
			std::vector<std::string> lodMeshes = meshes[i]["lodMeshes"].get<std::vector<std::string>>();
			meshObject->vecLODMeshs.push_back(sLODInfo(lodMeshes[0], 1000.0f));
			meshObject->vecLODMeshs.push_back(sLODInfo(lodMeshes[1], 200.0f));
			meshObject->vecLODMeshs.push_back(sLODInfo(lodMeshes[2], 100.0f));
		}


		vecObjectsToDraw.push_back(meshObject);
	}
}


void cSceneUtils::initializeCamera()
{
	nlohmann::json json;
	if (loadFromSaveFile)
	{
		std::ifstream ifs("savefile.json");
		json = json::parse(ifs);
		ifs.close();
	}
	else
	{
		json = cJsonUtils::getJsonInstance();
	}

	cCamera* camera = cCamera::getInstance();
	camera->eye.x = json["cameraEye"]["x"].get<float>();
	camera->eye.y = json["cameraEye"]["y"].get<float>();
	camera->eye.z = json["cameraEye"]["z"].get<float>();

	glm::vec3 lookAt;
	lookAt.x = json["cameraAt"]["x"].get<float>();
	lookAt.y = json["cameraAt"]["y"].get<float>();
	lookAt.z = json["cameraAt"]["z"].get<float>();

	camera->lookAt(lookAt);
}

void cSceneUtils::drawObject(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, GLuint shaderProgramID)
{
	cMeshObject* currentMesh = (cMeshObject*) pCurrentMesh;

	if (!currentMesh->isVisible)
		return;

	// Set up the texture binding for this object
	bindTextures(currentMesh, shaderProgramID);

	glm::mat4 matModelInvTrans;
	applyTranformations(currentMesh, matModel, matModelInvTrans);

	glUseProgram(shaderProgramID);

	GLint isToon_Uniloc = glGetUniformLocation(shaderProgramID, "toon");

	GLint isReflective_Uniloc = glGetUniformLocation(shaderProgramID, "isReflective");
	GLint isRefractive_Uniloc = glGetUniformLocation(shaderProgramID, "isRefractive");

	GLint water1_Uniloc = glGetUniformLocation(shaderProgramID, "isWater1");
	GLint water2_Uniloc = glGetUniformLocation(shaderProgramID, "isWater2");

	GLint objectDiffuse_UniLoc = glGetUniformLocation(shaderProgramID, "objectDiffuse");
	GLint objectSpecular_UniLoc = glGetUniformLocation(shaderProgramID, "objectSpecular");

	GLint useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");

	GLint matModel_location = glGetUniformLocation(shaderProgramID, "matModel");
	GLint matModelInvTrans_location = glGetUniformLocation(shaderProgramID, "matModelInvTrans");
	GLint matView_location = glGetUniformLocation(shaderProgramID, "matView");
	GLint matProj_location = glGetUniformLocation(shaderProgramID, "matProj");

	GLint bDontUseLighting_UniLoc = glGetUniformLocation(shaderProgramID, "bDontUseLighting");

	glUniformMatrix4fv(matModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(matModelInvTrans_location, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	/***************Blender Alpha Trans**************/
	// I'll do quick sort or whatever sexy sorts
	// One pass of bubble sort is fine...

	// Enable blend or "alpha" transparency
	glEnable(GL_BLEND);

	//glDisable( GL_BLEND );
	// Source == already on framebuffer
	// Dest == what you're about to draw
	// This is the "regular alpha blend transparency"
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(shaderProgramID,
		"wholeObjectAlphaTransparency");

	glUniform1f(wholeObjectAlphaTransparency_LocID, currentMesh->materialDiffuse.a);
	/***********************************************/

	glUniform4f(objectDiffuse_UniLoc,
		currentMesh->materialDiffuse.r,
		currentMesh->materialDiffuse.g,
		currentMesh->materialDiffuse.b,
		currentMesh->materialDiffuse.a);
	glUniform4f(objectSpecular_UniLoc,
		currentMesh->materialSpecular.r,
		currentMesh->materialSpecular.g,
		currentMesh->materialSpecular.b,
		currentMesh->materialSpecular.a);

	if (currentMesh->toon)
	{
		glUniform1f(isToon_Uniloc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(isToon_Uniloc, (float)GL_FALSE);
	}

	if (currentMesh->isReflective)
	{
		glUniform1f(isReflective_Uniloc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(isReflective_Uniloc, (float)GL_FALSE);
	}

	if (currentMesh->isRefractive)
	{
		glUniform1f(isRefractive_Uniloc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(isRefractive_Uniloc, (float)GL_FALSE);
	}

	if (currentMesh->friendlyName == "water1")
	{
		glUniform1f(water1_Uniloc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(water1_Uniloc, (float)GL_FALSE);
	}

	if (currentMesh->friendlyName == "water2")
	{
		glUniform1f(water2_Uniloc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(water2_Uniloc, (float)GL_FALSE);
	}

	if (currentMesh->useVertexColor)
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (currentMesh->dontLight)
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_FALSE);
	}

	if (currentMesh->isWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}

	//skinned mesh
	GLint bIsASkinnedMesh_LocID = glGetUniformLocation(shaderProgramID,
		"bIsASkinnedMesh");


	//!skinned mesh
	cModelDrawInfo modelInfo;
	if (currentMesh->pSimpleSkinnedMesh == NULL)
	{
		// It's a "regular" mesh
		modelInfo.meshFileName = currentMesh->meshName;

		if (currentMesh->vecLODMeshs.size() != 0)
		{
			float camDistance = glm::distance(cCamera::getInstance()->eye, currentMesh->position);

			std::vector<sLODInfo>::iterator it = currentMesh->vecLODMeshs.begin();
			modelInfo.meshFileName = it->meshName;

			for (; it != currentMesh->vecLODMeshs.end(); it++)
			{
				if (camDistance < it->minDistance)
				{
					modelInfo.meshFileName = it->meshName;
				}
			}
		}
		glUniform1f(bIsASkinnedMesh_LocID, (float)GL_FALSE);
	}
	else
	{
		// It ++IS++ skinned mesh
		modelInfo.meshFileName = currentMesh->pSimpleSkinnedMesh->fileName;

		glUniform1f(bIsASkinnedMesh_LocID, (float)GL_TRUE);

		// Also pass up the bone information...
		std::vector< glm::mat4x4 > vecFinalTransformation;	// Replaced by	theMesh.vecFinalTransformation
		std::vector< glm::mat4x4 > vecOffsets;

		//		cAnimationState* pAniState = pCurrentMesh->pAniState->;
				// Are there any animations in the queue?
		//		if ( pCurrentMesh->pAniState->vecAnimationQueue.empty() )
		float timeToUse = g_HACK_CurrentTime;
		if (shouldUpdate && currentMesh->currentAnimation == "Jump")
		{
			timeToUse = walkAnimTime;
		}
		else
		{
			timeToUse = g_HACK_CurrentTime;
		}
		currentMesh->pSimpleSkinnedMesh->BoneTransform(
											//0.0f,	// curFrameTime,
											//g_HACK_CurrentTime,	// curFrameTime,
											//walkAnimTime,
											timeToUse,
											//"assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX",		// animationToPlay,		//**NEW**
											//"assets/modelsFBX/RPG-Character_Unarmed-Roll-Backward(FBX2013).fbx",		// animationToPlay,		//**NEW**
											//"assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx",		// animationToPlay,		//**NEW**
											currentMesh->currentAnimation,
											vecFinalTransformation,		// Final bone transforms for mesh
											currentMesh->vecObjectBoneTransformation,  // final location of bones
											vecOffsets);                 // local offset for each bone


		g_HACK_CurrentTime += 0.003f;		// Frame time, but we are going at 60HZ
		if (walkAnimTime <= 1.0f)
		{
			walkAnimTime += 0.003f;
		}
		//std::cout << "walkanim: " << walkAnimTime << std::endl;

		unsigned int numberOfBonesUsed = static_cast<unsigned int>(vecFinalTransformation.size());

		GLint numBonesUsed_UniLoc = glGetUniformLocation(shaderProgramID, "numBonesUsed");
		glUniform1i(numBonesUsed_UniLoc, numberOfBonesUsed);

		//		const unsigned int TOTALNUMBEROFBONESTOPASSINTOTHESHADERASIDENTIYMATRIXVALUES = 99;
		//		for ( unsigned int index = 0; index != numberOfBonesUsed; index++ )
		//		{
		//			vecFinalTransformation.push_back( glm::mat4(1.0f) );
		//		}

		glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);

		// UniformLoc_bonesArray is the getUniformLoc of "bones[0]" from
		//	uniform mat4 bones[MAXNUMBEROFBONES] 
		// in the shader
		GLint bones_UniLoc = glGetUniformLocation(shaderProgramID, "bones");
		//		std::cout << "bones_UniLoc: " << bones_UniLoc << std::endl;	std::cout.flush();
		glUniformMatrix4fv(bones_UniLoc, numberOfBonesUsed, GL_FALSE,
			(const GLfloat*)glm::value_ptr(*pBoneMatrixArray));

		// Update the extents of the skinned mesh from the bones...
		//	sMeshDrawInfo.minXYZ_from_SM_Bones(glm::vec3(0.0f)), 
		//  sMeshDrawInfo.maxXYZ_from_SM_Bones(glm::vec3(0.0f))
		for (unsigned int boneIndex = 0; boneIndex != numberOfBonesUsed; boneIndex++)
		{
			glm::mat4 boneLocal = currentMesh->vecObjectBoneTransformation[boneIndex];

			// HACK: Need to add "uniform scale" to mesh
	//		float scale = pCurrentMesh->nonUniformScale.x;
			float scale = 1.0f;	// For now
			boneLocal = glm::scale(boneLocal, glm::vec3(currentMesh->scale,
				currentMesh->scale,
				currentMesh->scale));

			glm::vec4 boneBallLocation = boneLocal * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			boneBallLocation *= scale;

			// Draw a debug sphere at the location of the bone
	//		boneBallLocation += glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);


	//		::g_pDebugRenderer->addDebugSphere( glm::vec3(boneBallLocation), 
	//										    glm::vec3( 1.0f, 1.0f, 1.0f ),
	//										    scale, 0.0f );
			//DrawDebugBall( glm::vec3(boneBallLocation), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.2f );


			// Bone index [13] = "B_L_Finger31"
			if (boneIndex == 25)
			{
				//DrawDebugBall( glm::vec3(boneBallLocation), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.5f );
	//			std::cout << "Bone 13, B_L_Finger31: " 
	//				<< boneBallLocation.x << ", "
	//				<< boneBallLocation.y << ", " 
	//				<< boneBallLocation.z << std::endl;

				/*cMeshObject* pPlayerBody = (cMeshObject*) findObjectByFriendlyName("PlayerBody");
				pPlayerBody->scale = 10.0f;
				pPlayerBody->position = boneBallLocation;*/
			}




			// Update the extents of the mesh
			if (boneIndex == 0)
			{
				// For the 0th bone, just assume this is the extent
				currentMesh->minXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
				currentMesh->maxXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
			}
			else
			{	// It's NOT the 0th bone, so compare with current max and min
				if (currentMesh->minXYZ_from_SM_Bones.x < boneBallLocation.x) { currentMesh->minXYZ_from_SM_Bones.x = boneBallLocation.x; }
				if (currentMesh->minXYZ_from_SM_Bones.y < boneBallLocation.y) { currentMesh->minXYZ_from_SM_Bones.y = boneBallLocation.y; }
				if (currentMesh->minXYZ_from_SM_Bones.z < boneBallLocation.z) { currentMesh->minXYZ_from_SM_Bones.z = boneBallLocation.z; }

				if (currentMesh->maxXYZ_from_SM_Bones.x > boneBallLocation.x) { currentMesh->maxXYZ_from_SM_Bones.x = boneBallLocation.x; }
				if (currentMesh->maxXYZ_from_SM_Bones.y > boneBallLocation.y)
				{
					currentMesh->maxXYZ_from_SM_Bones.y = boneBallLocation.y;
				}
				if (currentMesh->maxXYZ_from_SM_Bones.z > boneBallLocation.z)
				{
					currentMesh->maxXYZ_from_SM_Bones.z = boneBallLocation.z;
				}
			}//if ( boneIndex == 0 )


		}
	}



	if (cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelInfo))
	{
		//glDrawArrays(GL_TRIANGLES, 0, bunnyInfo.numberOfIndices );

		glBindVertexArray(modelInfo.VAO_ID);

		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);

		glBindVertexArray(0);

	}

	return;
}

void cSceneUtils::bindTextures(cMeshObject* pCurrentMesh, GLuint shaderProgramID)
{
	// This is pretty much a hack, so we should likely pass the shader object 
	// (pointer) to this function, and to the DrawObject call, too. 
	// (Another option would be to pass the shader MANAGER instead, so 
	//  that the functions can look up various things in the shader)
	//
	// For now, I'm going to get the uniform location here 
	// (to make it clear (maybe?) that we'll NEED those shader uniform locations)

	// So this is only called once... 
	if (!HACK_bTextureUniformLocationsLoaded)
	{
		tex00_UniLoc = glGetUniformLocation(shaderProgramID, "texture00");		// uniform sampler2D texture00;
		tex01_UniLoc = glGetUniformLocation(shaderProgramID, "texture01");		// uniform sampler2D texture01;
		tex02_UniLoc = glGetUniformLocation(shaderProgramID, "texture02");		// uniform sampler2D texture02;
		tex03_UniLoc = glGetUniformLocation(shaderProgramID, "texture03");		// uniform sampler2D texture03;
		tex04_UniLoc = glGetUniformLocation(shaderProgramID, "texture04");		// uniform sampler2D texture04;
		tex05_UniLoc = glGetUniformLocation(shaderProgramID, "texture05");		// uniform sampler2D texture05;
		tex06_UniLoc = glGetUniformLocation(shaderProgramID, "texture06");		// uniform sampler2D texture06;
		tex07_UniLoc = glGetUniformLocation(shaderProgramID, "texture07");		// uniform sampler2D texture07;

		texBW_0_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[0]");	// uniform vec4 texBlendWeights[2];
		texBW_1_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[1]");	// uniform vec4 texBlendWeights[2];

		texPass1OutputTexture_UniLoc = glGetUniformLocation(shaderProgramID, "texPass1OutputTexture");

		HACK_bTextureUniformLocationsLoaded = true;

	}//if(!HACK_bTextureUniformLocationsLoaded )
	float blendWeights[8] = { 0 };
	if (pCurrentMesh->b_HACK_UsesOffscreenFBO)
	{
		// Connect the texture for this object to the FBO texture
		// Pick texture unit 16 (just because - I randomly picked that)

		int FBO_Texture_Unit_Michael_Picked = 1;

		// 0x84C0  (or 33984)		
		// Please bind to texture unit 34,000. Why gawd, why?
		glActiveTexture(GL_TEXTURE0 + FBO_Texture_Unit_Michael_Picked);

		// Connect the specific texture to THIS texture unit
//		glBindTexture( GL_TEXTURE_2D, g_FBO_colourTexture );
		glBindTexture(GL_TEXTURE_2D, g_pFBOMain->colourTexture_0_ID);

		// Now pick to read from the normal (output from the 1st pass):
//		glBindTexture( GL_TEXTURE_2D, ::g_pFBOMain->normalTexture_1_ID );
//		glBindTexture( GL_TEXTURE_2D, ::g_pFBOMain->depthTexture_ID );
//		glBindTexture( GL_TEXTURE_2D, ::g_pFBOMain->vertexWorldPos_2_ID );


		// Set the sampler (in the shader) to ALSO point to texture unit 16
		// This one takes the unchanged texture unit numbers 
//		glUniform1i( tex00_UniLoc, FBO_Texture_Unit_Michael_Picked );
		glUniform1i(texPass1OutputTexture_UniLoc, FBO_Texture_Unit_Michael_Picked);


		// Set the blending to that it's 0th texture sampler
		// NOTE: it's only the 0th (1st) texture that we are mixing from
//		glUniform4f( texBW_0_UniLoc, 1.0f, 0.0f, 0.0f, 0.0f );		// <---- Note the 1.0f
//		glUniform4f( texBW_1_UniLoc, 0.0f, 0.0f, 0.0f, 0.0f );

		// NOTE: Early return (so we don't set any other textures
		// Again; HACK!!
		//reticle texture
		if (drawReticle)
		{
			FBO_Texture_Unit_Michael_Picked++;
			glActiveTexture(GL_TEXTURE0 + FBO_Texture_Unit_Michael_Picked);

			std::string texName = pCurrentMesh->vecTextures[0].name;
			GLuint texID = cBasicTextureManager::getInstance()->getTextureIDFromName(texName);

			glBindTexture(GL_TEXTURE_2D, texID);
			glUniform1i(tex02_UniLoc, FBO_Texture_Unit_Michael_Picked);

			blendWeights[2] = pCurrentMesh->vecTextures[0].strength;
			glUniform4f(texBW_0_UniLoc, blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
			glUniform4f(texBW_1_UniLoc, blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);
		}
		return;
	}//if ( pCurrentMesh->b_HACK_UsesOffscreenFBO )


	// For each texture, bind the texture to a texture unit and sampler
	// Texture #0 (on the mesh) -- Texture Unit 0 -- Sampler 0
	// Texture #1 (on the mesh) -- Texture Unit 1 -- Sampler 1
	// ....

	// Set all the blend weights (strengths) to zero


	for (int texBindIndex = 0; texBindIndex != pCurrentMesh->vecTextures.size(); texBindIndex++)
	{
		// Bind to the the "texBindIndex" texture unit
		glActiveTexture(GL_TEXTURE0 + texBindIndex);

		// Connect the specific texture to THIS texture unit
		std::string texName = pCurrentMesh->vecTextures[texBindIndex].name;

		GLuint texID = cBasicTextureManager::getInstance()->getTextureIDFromName(texName);

		glBindTexture(GL_TEXTURE_2D, texID);

		// Use a switch to pick the texture sampler and weight (strength)
		// BECAUSE the samplers can't be in an array
		switch (texBindIndex)
		{
		case 0:		// uniform sampler2D texture00  AND texBlendWeights[0].x;
			glUniform1i(tex00_UniLoc, texBindIndex);
			break;
		case 1:		// uniform sampler2D texture01  AND texBlendWeights[0].y;
			glUniform1i(tex01_UniLoc, texBindIndex);
			break;
		case 2:
			glUniform1i(tex02_UniLoc, texBindIndex);
			break;
		case 3:
			glUniform1i(tex03_UniLoc, texBindIndex);
			break;
		case 4:		// uniform sampler2D texture04  AND texBlendWeights[1].x;
			glUniform1i(tex04_UniLoc, texBindIndex);
			break;
		case 5:
			glUniform1i(tex05_UniLoc, texBindIndex);
			break;
		case 6:
			glUniform1i(tex06_UniLoc, texBindIndex);
			break;
		case 7:
			glUniform1i(tex07_UniLoc, texBindIndex);
			break;
		}//switch ( texBindIndex )

		// Set the blend weight (strengty)
		blendWeights[texBindIndex] = pCurrentMesh->vecTextures[texBindIndex].strength;

	}//for ( int texBindIndex

	// Set the weights (strengths) in the shader
	glUniform4f(texBW_0_UniLoc, blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
	glUniform4f(texBW_1_UniLoc, blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);

	return;
}

void cSceneUtils::applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, glm::mat4& matModelInvTrans)
{
	cMeshObject* currentMesh = (cMeshObject*) pCurrentMesh;
	matModel = glm::mat4x4(1.0f);		// mat4x4_identity(m);

	if (currentMesh->rigidBody != 0)
	{
		currentMesh->rigidBody->GetTransform(matModel);
		glm::quat rotation = currentMesh->getOrientation();

		glm::mat4 matRotation = glm::mat4(rotation);

		matModel = matModel * matRotation;

		if (currentMesh->friendlyName == "SM_Object")
		{
			glm::vec4 pos = matModel[3];
			pos.y -= 10.0f;
			matModel[3] = pos;
		}
	}
	else
	{

		glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
			currentMesh->position);

		matModel = matModel * matTranslation;		// matMove

		//glm::mat4 postRot_X = glm::rotate(glm::mat4(1.0f),
		//	currentMesh->postRotation.x,
		//	glm::vec3(1.0f, 0.0, 0.0f));
		//matModel = matModel * postRot_X;

		//glm::mat4 postRot_Y = glm::rotate(glm::mat4(1.0f),
		//	currentMesh->postRotation.y,
		//	glm::vec3(0.0f, 1.0, 0.0f));
		//matModel = matModel * postRot_Y;

		//glm::mat4 postRot_Z = glm::rotate(glm::mat4(1.0f),
		//	currentMesh->postRotation.z,
		//	glm::vec3(0.0f, 0.0, 1.0f));
		//matModel = matModel * postRot_Z;

		glm::quat rotation = currentMesh->getOrientation();

		glm::mat4 matRotation = glm::mat4(rotation);

		matModel = matModel * matRotation;
	}

	// And now scale
	matModelInvTrans = glm::inverse(glm::transpose(matModel));

	float scale = currentMesh->scale;
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	matModel = matModel * matScale;
}

void cSceneUtils::selectNextMeshObject(bool includeInvisibleObject)
{
	if (selectedObjectIndex >= vecObjectsToDraw.size() - 1)
	{
		selectedObjectIndex = 0;
	}
	else
	{
		selectedObjectIndex++;
	}
	selectedMeshObject = vecObjectsToDraw[selectedObjectIndex];
	std::cout << ((cMeshObject*)selectedMeshObject)->friendlyName << " selected, isVisible : " << ((cMeshObject*)selectedMeshObject)->isVisible << std::endl;
}

void cSceneUtils::drawSkyBox(glm::vec3 eye, GLuint program)
{
	cMeshObject* skyBox = (cMeshObject*) findObjectByFriendlyName("SkyBoxObject");

	skyBox->position = eye;
	skyBox->isVisible = true;
	skyBox->isWireFrame = false;

	GLuint cityTextureUNIT_ID = 30;			// Texture unit go from 0 to 79
	glActiveTexture(cityTextureUNIT_ID + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

	int cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("CityCubeMap");
	// Cube map is now bound to texture unit 30
	//		glBindTexture( GL_TEXTURE_2D, cubeMapTextureID );
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

	//uniform samplerCube textureSkyBox;
	GLint skyBoxCubeMap_UniLoc = glGetUniformLocation(program, "textureSkyBox");
	glUniform1i(skyBoxCubeMap_UniLoc, cityTextureUNIT_ID);

	cityTextureUNIT_ID++;
	glActiveTexture(cityTextureUNIT_ID + GL_TEXTURE0);
	int cubeMapTextureID2 = cBasicTextureManager::getInstance()->getTextureIDFromName("SpaceCubeMap");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID2);

	GLint spaceSkyBoxCubeMap_UniLoc = glGetUniformLocation(program, "textureSpace");
	glUniform1i(spaceSkyBoxCubeMap_UniLoc, cityTextureUNIT_ID);

	//uniform bool useSkyBoxTexture;
	GLint useSkyBoxTexture_UniLoc = glGetUniformLocation(program, "useSkyBoxTexture");
	glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_TRUE);

	glm::mat4 matIdentity = glm::mat4(1.0f);
	drawObject(skyBox, matIdentity, program);

	//		glEnable( GL_CULL_FACE );
	//		glCullFace( GL_BACK );

	skyBox->isVisible = false;
	glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_FALSE);
}

void cSceneUtils::drawAABBs(GLuint program)
{

	if (!this->showAABBs)
	{
		return;
	}

	cMeshObject* aabbCube = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cube");
	aabbCube->isVisible = true;
	aabbCube->dontLight = true;
	aabbCube->isWireFrame = true;

	glm::mat4 matCube(1.0f);

	terrainHierarchy->mapAABBs;

	for (std::map<unsigned long long, cAABB*>::iterator it = terrainHierarchy->mapAABBs.begin(); it != terrainHierarchy->mapAABBs.end(); it++)
	{
		 aabbCube->position = it->second->getCentre();
		 aabbCube->scale = it->second->getHalfSideLength() / 10.0f;
		 drawObject(aabbCube, matCube, program);
	}

	aabbCube->isVisible = false;
}

bool cSceneUtils::AssimpSM_to_VAO_Converter(cSimpleAssimpSkinnedMesh * pTheAssimpSM, unsigned int shaderProgramID)
{
	cModelDrawInfo theSMDrawInfo;

	theSMDrawInfo.meshFileName = pTheAssimpSM->fileName;
	theSMDrawInfo.friendlyName = pTheAssimpSM->friendlyName;

	// Copy the data from assimp format into the sModelDrawInfo format...

	theSMDrawInfo.numberOfVertices = pTheAssimpSM->pScene->mMeshes[0]->mNumVertices;
	theSMDrawInfo.numberOfTriangles = pTheAssimpSM->pScene->mMeshes[0]->mNumFaces;

	// We used the "triangulate" option when loading so all the primitives
	//	will be triangles, but BEWARE!
	theSMDrawInfo.numberOfIndices = theSMDrawInfo.numberOfTriangles * 3;


	// Allocate the vertex array (it's a c-style array)
	theSMDrawInfo.pMeshData = new cMesh();

	theSMDrawInfo.pMeshData->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theSMDrawInfo.numberOfVertices];

	// Danger Will Robinson! 
	// You don't really need to do this, but at least it will clear it to zero.
	// (compiler will complain that it's 'not safe', etc.)
	memset(theSMDrawInfo.pMeshData->pVertices, 0,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * theSMDrawInfo.numberOfVertices);


	for (unsigned int vertIndex = 0; vertIndex != theSMDrawInfo.numberOfVertices; vertIndex++)
	{
		theSMDrawInfo.pMeshData->pVertices[vertIndex].x
			= pTheAssimpSM->pScene->mMeshes[0]->mVertices[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].y
			= pTheAssimpSM->pScene->mMeshes[0]->mVertices[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].z
			= pTheAssimpSM->pScene->mMeshes[0]->mVertices[vertIndex].z;

		// Normals... 
		theSMDrawInfo.pMeshData->pVertices[vertIndex].nx
			= pTheAssimpSM->pScene->mMeshes[0]->mNormals[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].ny
			= pTheAssimpSM->pScene->mMeshes[0]->mNormals[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].nz
			= pTheAssimpSM->pScene->mMeshes[0]->mNormals[vertIndex].z;

		// Colours... 
		// (If there are no colours, make it hit pink)
		// Note: the method is because you could have more than one set of 
		//	vertex colours in the model (Why? Who the heck knows?) 
		if (pTheAssimpSM->pScene->mMeshes[0]->HasVertexColors(0))
		{
			theSMDrawInfo.pMeshData->pVertices[vertIndex].r
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->r;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].g
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->g;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].b
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->b;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].a
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->a;
		}
		else
		{	// hotpink	#FF69B4	rgb(255,105,180)
			theSMDrawInfo.pMeshData->pVertices[vertIndex].r = 1.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].g = 105.0f / 255.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].b = 180.0f / 255.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].a = 1.0f;
		}

		//bi-normal  (or bi-tangent)
		theSMDrawInfo.pMeshData->pVertices[vertIndex].bx
			= pTheAssimpSM->pScene->mMeshes[0]->mBitangents[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].by
			= pTheAssimpSM->pScene->mMeshes[0]->mBitangents[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].bz
			= pTheAssimpSM->pScene->mMeshes[0]->mBitangents[vertIndex].z;

		// Tangent
		theSMDrawInfo.pMeshData->pVertices[vertIndex].tx
			= pTheAssimpSM->pScene->mMeshes[0]->mTangents[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].ty
			= pTheAssimpSM->pScene->mMeshes[0]->mTangents[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].tz
			= pTheAssimpSM->pScene->mMeshes[0]->mTangents[vertIndex].z;

		// uv2 (which are odd in assimp)
		// Note that there is an array of texture coordinates, 
		// up to 8 (actually). Usually, there's only 1
		if (pTheAssimpSM->pScene->mMeshes[0]->HasTextureCoords(0))	// 1st UV coords
		{
			// Assume there's 1... (at least)
			theSMDrawInfo.pMeshData->pVertices[vertIndex].u0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[0]->x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[0]->y;
		}
		if (pTheAssimpSM->pScene->mMeshes[0]->HasTextureCoords(1))	// 2nd UV coords
		{
			theSMDrawInfo.pMeshData->pVertices[vertIndex].u0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[1]->x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[1]->y;
		}
		// TODO: add additional texture coordinates (mTextureCoords[1], etc.)


		// 4Bones: ids and weights
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[0] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[0];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[1] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[1];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[2] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[2];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[3] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[3];

		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[0] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[0];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[1] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[1];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[2] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[2];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[3] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[3];


	}//for ( unsigned int vertIndex = 0;

	// And the triangles

	// Allocate the array to hold the indices (triangle) info


	// Allocate the array for that (indices NOT triangles)
	theSMDrawInfo.pMeshData->pIndices = new unsigned int[theSMDrawInfo.numberOfIndices];

	// Danger Will Robinson! 
	// You don't really need to do this, but at least it will clear it to zero.
	// (compiler will complain that it's 'not safe', etc.)
	memset(theSMDrawInfo.pMeshData->pIndices, 0,
		sizeof(unsigned int) * theSMDrawInfo.numberOfIndices);


	unsigned int numTriangles = pTheAssimpSM->pScene->mMeshes[0]->mNumFaces;
	unsigned int triIndex = 0;		// Steps through the triangles.
	unsigned int indexIndex = 0;	// Setps through the indices (index buffer)
	for (; triIndex != theSMDrawInfo.numberOfTriangles;
		triIndex++, indexIndex += 3)	// Note, every 1 triangle = 3 index steps
	{
		// Get the triangle at this triangle index...
		aiFace* pAIFace = &(pTheAssimpSM->pScene->mMeshes[0]->mFaces[triIndex]);

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 0]				// Offset by 0 (zero)
			= pAIFace->mIndices[0];	// vertex 0

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 1]				// Offset by 1
			= pAIFace->mIndices[1];	// vertex 1

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 2]				// Offset by 2
			= pAIFace->mIndices[2];	// vertex 1
	}//for ( ; triIndex != numVertices; 


	// Calculate the extents on the mesh
	// (Note, because I'm a bone head, this is dupicated...)
	theSMDrawInfo.pMeshData->name = theSMDrawInfo.friendlyName;
	theSMDrawInfo.pMeshData->numberOfIndices = theSMDrawInfo.numberOfIndices;
	theSMDrawInfo.pMeshData->numberOfTriangles = theSMDrawInfo.numberOfTriangles;
	theSMDrawInfo.pMeshData->numberOfVertices = theSMDrawInfo.numberOfVertices;
	theSMDrawInfo.pMeshData->CalculateExtents();

	// ...then pass it to the mesh manager

	cVAOMeshUtils::getInstance()->loadModelIntoVAO(theSMDrawInfo, shaderProgramID);
	return true;
}

void cSceneUtils::LoadSkinnedMeshModel(GLuint shaderProgramID)
{
	{
		cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

		//if (!g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX"))
		if (!g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/paladin_j_nordstrom.fbx"))
		{
			std::cout << "Error: problem loading the skinned mesh" << std::endl;
		}
		std::vector<std::string> vecBoneNames;
		g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

		// Now load another animation file... 
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/pjn_idle.fbx");
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/pjn_walking.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Walk-Back", "assets/modelsFBX/pjn_walking_back.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Right", "assets/modelsFBX/pjn_strafe_right.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Left", "assets/modelsFBX/pjn_strafe_left.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Action", "assets/modelsFBX/pjn_punch.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/pjn_jump.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Run", "assets/modelsFBX/pjn_run.fbx");

		g_pRPGSkinnedMesh->friendlyName = "RPG-Character";

		cMesh* pTheMesh = g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

		if (pTheMesh)
		{
			std::cout << "Mesh got loaded" << std::endl;
		}
		else
		{
			std::cout << "Didn't load the skinned mesh model" << std::endl;
		}

		// Copy the mesh information from assimp into our cMesh object, 
		// then into the sModelDrawInfo thing, and pass to the VAOManager

		if (!AssimpSM_to_VAO_Converter(g_pRPGSkinnedMesh, shaderProgramID))
		{
			std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
		}
		else
		{
			std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

			// Add this mesh model into the "models to draw" vector

			{	// Bind pose Skinned Mesh object
				cMeshObject* pTestSM = new cMeshObject();
				pTestSM->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));	// Yellow
				pTestSM->setAlphaTransparency(1.0f);
				pTestSM->friendlyName = "SM_Object";
				pTestSM->position = glm::vec3(0.0f, 0.0f, 0.0f);
				pTestSM->scale = 1.0f;
				pTestSM->meshName = "RPG-Character.ply";

				// Wireframe, to make it easier to see (for now)
	//			pTestSM->bIsWireFrame = true;
	//			pTestSM->bDontLight = true;

				// Set this mesh to the skinned mesh object
				pTestSM->pSimpleSkinnedMesh = g_pRPGSkinnedMesh;
				// HACK
				//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
	//			pTestSM->currentAnimation = "Idle";

				cAnimationState* pAniState;
				pTestSM->pAniState = new cAnimationState();

				pTestSM->pAniState->defaultAnimation.name = "Idle";
				pTestSM->currentAnimation = "Idle";


				sTextureInfo testObjectTexture;
				testObjectTexture.name = "grass.bmp";
				testObjectTexture.strength = 1.0f;

				pTestSM->vecTextures.push_back(sTextureInfo(testObjectTexture));
				pTestSM->scale = 1.0f;
				vecObjectsToDraw.push_back(pTestSM);
			}
		}//if ( ! AssimpSM_to_VAO_Converter(
	}

	{
		cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

		//if (!g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX"))
		if (!g_pRPGSkinnedMesh->LoadMeshFromFile("GirlCharacter", "assets/modelsFBX/girl.fbx"))
		{
			std::cout << "Error: problem loading the skinned mesh" << std::endl;
		}
		std::vector<std::string> vecBoneNames;
		g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

		// Now load another animation file... 
		//g_pRPGSkinnedMesh->LoadMeshAnimation("IdleGirl", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/girl_idle.fbx");
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-WalkGirl", "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/girl_walking.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Walk-Back", "assets/modelsFBX/girl_walking_back.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Right", "assets/modelsFBX/girl_strafe_right.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Left", "assets/modelsFBX/girl_strafe_left.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Action", "assets/modelsFBX/girl_punch.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/girl_jump.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Run", "assets/modelsFBX/girl_run.fbx");

		g_pRPGSkinnedMesh->friendlyName = "GirlCharacter";

		cMesh* pTheMesh = g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

		if (pTheMesh)
		{
			std::cout << "Mesh got loaded" << std::endl;
		}
		else
		{
			std::cout << "Didn't load the skinned mesh model" << std::endl;
		}

		// Copy the mesh information from assimp into our cMesh object, 
		// then into the sModelDrawInfo thing, and pass to the VAOManager

		if (!AssimpSM_to_VAO_Converter(g_pRPGSkinnedMesh, shaderProgramID))
		{
			std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
		}
		else
		{
			std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

			// Add this mesh model into the "models to draw" vector

			{	// Bind pose Skinned Mesh object
				cMeshObject* pTestSM = new cMeshObject();
				pTestSM->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));	// Yellow
				pTestSM->setAlphaTransparency(1.0f);
				pTestSM->friendlyName = "GirlCharacter";
				pTestSM->position = glm::vec3(0.0f, 0.0f, 0.0f);
				pTestSM->scale = 0.1f;
				pTestSM->meshName = "GirlCharacter.ply";

				// Wireframe, to make it easier to see (for now)
	//			pTestSM->bIsWireFrame = true;
	//			pTestSM->bDontLight = true;

				// Set this mesh to the skinned mesh object
				pTestSM->pSimpleSkinnedMesh = g_pRPGSkinnedMesh;
				// HACK
				//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
	//			pTestSM->currentAnimation = "Idle";

				cAnimationState* pAniState;
				pTestSM->pAniState = new cAnimationState();

				pTestSM->pAniState->defaultAnimation.name = "Idle";

				pTestSM->currentAnimation = "Idle";

				sTextureInfo testObjectTexture;
				testObjectTexture.name = "grass.bmp";
				testObjectTexture.strength = 1.0f;

				pTestSM->vecTextures.push_back(sTextureInfo(testObjectTexture));
				vecObjectsToDraw.push_back(pTestSM);
			}
		}//if ( ! AssimpSM_to_VAO_Converter(
	}

	{
		cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

		//if (!g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX"))
		if (!g_pRPGSkinnedMesh->LoadMeshFromFile("NPCCharacter1", "assets/modelsFBX/paladin_j_nordstrom.fbx"))
		{
			std::cout << "Error: problem loading the skinned mesh" << std::endl;
		}
		std::vector<std::string> vecBoneNames;
		g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

		// Now load another animation file... 
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/pjn_idle.fbx");
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/pjn_walking.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Walk-Back", "assets/modelsFBX/pjn_walking_back.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Right", "assets/modelsFBX/pjn_strafe_right.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Left", "assets/modelsFBX/pjn_strafe_left.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Action", "assets/modelsFBX/pjn_punch.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/pjn_jump.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Run", "assets/modelsFBX/pjn_run.fbx");

		g_pRPGSkinnedMesh->friendlyName = "NPCCharacter1";

		cMesh* pTheMesh = g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

		if (pTheMesh)
		{
			std::cout << "Mesh got loaded" << std::endl;
		}
		else
		{
			std::cout << "Didn't load the skinned mesh model" << std::endl;
		}

		// Copy the mesh information from assimp into our cMesh object, 
		// then into the sModelDrawInfo thing, and pass to the VAOManager

		if (!AssimpSM_to_VAO_Converter(g_pRPGSkinnedMesh, shaderProgramID))
		{
			std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
		}
		else
		{
			std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

			// Add this mesh model into the "models to draw" vector

			{	// Bind pose Skinned Mesh object
				cMeshObject* pTestSM = new cMeshObject();
				pTestSM->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));	// Yellow
				pTestSM->setAlphaTransparency(1.0f);
				pTestSM->friendlyName = "NPCCharacter1";
				pTestSM->position = glm::vec3(0.0f, 0.0f, 0.0f);
				pTestSM->scale = 1.0f;
				pTestSM->meshName = "RPG-Character.ply";

				// Wireframe, to make it easier to see (for now)
	//			pTestSM->bIsWireFrame = true;
	//			pTestSM->bDontLight = true;

				// Set this mesh to the skinned mesh object
				pTestSM->pSimpleSkinnedMesh = g_pRPGSkinnedMesh;
				// HACK
				//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
	//			pTestSM->currentAnimation = "Idle";

				cAnimationState* pAniState;
				pTestSM->pAniState = new cAnimationState();

				pTestSM->pAniState->defaultAnimation.name = "Idle";
				pTestSM->currentAnimation = "Idle";


				sTextureInfo testObjectTexture;
				testObjectTexture.name = "grass.bmp";
				testObjectTexture.strength = 1.0f;

				pTestSM->vecTextures.push_back(sTextureInfo(testObjectTexture));
				pTestSM->scale = 0.10f;
				vecObjectsToDraw.push_back(pTestSM);
			}
		}//if ( ! AssimpSM_to_VAO_Converter(
	}

	{
		cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

		//if (!g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX"))
		if (!g_pRPGSkinnedMesh->LoadMeshFromFile("NPCCharacter2", "assets/modelsFBX/paladin_j_nordstrom.fbx"))
		{
			std::cout << "Error: problem loading the skinned mesh" << std::endl;
		}
		std::vector<std::string> vecBoneNames;
		g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

		// Now load another animation file... 
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/pjn_idle.fbx");
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/pjn_walking.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Walk-Back", "assets/modelsFBX/pjn_walking_back.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Right", "assets/modelsFBX/pjn_strafe_right.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Left", "assets/modelsFBX/pjn_strafe_left.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Action", "assets/modelsFBX/pjn_punch.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/pjn_jump.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Run", "assets/modelsFBX/pjn_run.fbx");

		g_pRPGSkinnedMesh->friendlyName = "NPCCharacter2";

		cMesh* pTheMesh = g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

		if (pTheMesh)
		{
			std::cout << "Mesh got loaded" << std::endl;
		}
		else
		{
			std::cout << "Didn't load the skinned mesh model" << std::endl;
		}

		// Copy the mesh information from assimp into our cMesh object, 
		// then into the sModelDrawInfo thing, and pass to the VAOManager

		if (!AssimpSM_to_VAO_Converter(g_pRPGSkinnedMesh, shaderProgramID))
		{
			std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
		}
		else
		{
			std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

			// Add this mesh model into the "models to draw" vector

			{	// Bind pose Skinned Mesh object
				cMeshObject* pTestSM = new cMeshObject();
				pTestSM->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));	// Yellow
				pTestSM->setAlphaTransparency(1.0f);
				pTestSM->friendlyName = "NPCCharacter2";
				pTestSM->position = glm::vec3(0.0f, 0.0f, 0.0f);
				pTestSM->scale = 1.0f;
				pTestSM->meshName = "RPG-Character.ply";

				// Wireframe, to make it easier to see (for now)
	//			pTestSM->bIsWireFrame = true;
	//			pTestSM->bDontLight = true;

				// Set this mesh to the skinned mesh object
				pTestSM->pSimpleSkinnedMesh = g_pRPGSkinnedMesh;
				// HACK
				//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
	//			pTestSM->currentAnimation = "Idle";

				cAnimationState* pAniState;
				pTestSM->pAniState = new cAnimationState();

				pTestSM->pAniState->defaultAnimation.name = "Idle";
				pTestSM->currentAnimation = "Idle";


				sTextureInfo testObjectTexture;
				testObjectTexture.name = "grass.bmp";
				testObjectTexture.strength = 1.0f;

				pTestSM->vecTextures.push_back(sTextureInfo(testObjectTexture));
				pTestSM->scale = 0.10f;
				vecObjectsToDraw.push_back(pTestSM);
			}
		}//if ( ! AssimpSM_to_VAO_Converter(
	}

	{
		cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

		//if (!g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX"))
		if (!g_pRPGSkinnedMesh->LoadMeshFromFile("NPCCharacter3", "assets/modelsFBX/paladin_j_nordstrom.fbx"))
		{
			std::cout << "Error: problem loading the skinned mesh" << std::endl;
		}
		std::vector<std::string> vecBoneNames;
		g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

		// Now load another animation file... 
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/pjn_idle.fbx");
		//g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/pjn_walking.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Walk-Back", "assets/modelsFBX/pjn_walking_back.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Right", "assets/modelsFBX/pjn_strafe_right.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-Left", "assets/modelsFBX/pjn_strafe_left.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Action", "assets/modelsFBX/pjn_punch.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/pjn_jump.fbx");
		g_pRPGSkinnedMesh->LoadMeshAnimation("Run", "assets/modelsFBX/pjn_run.fbx");

		g_pRPGSkinnedMesh->friendlyName = "NPCCharacter3";

		cMesh* pTheMesh = g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

		if (pTheMesh)
		{
			std::cout << "Mesh got loaded" << std::endl;
		}
		else
		{
			std::cout << "Didn't load the skinned mesh model" << std::endl;
		}

		// Copy the mesh information from assimp into our cMesh object, 
		// then into the sModelDrawInfo thing, and pass to the VAOManager

		if (!AssimpSM_to_VAO_Converter(g_pRPGSkinnedMesh, shaderProgramID))
		{
			std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
		}
		else
		{
			std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

			// Add this mesh model into the "models to draw" vector

			{	// Bind pose Skinned Mesh object
				cMeshObject* pTestSM = new cMeshObject();
				pTestSM->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));	// Yellow
				pTestSM->setAlphaTransparency(1.0f);
				pTestSM->friendlyName = "NPCCharacter3";
				pTestSM->position = glm::vec3(0.0f, 0.0f, 0.0f);
				pTestSM->scale = 1.0f;
				pTestSM->meshName = "RPG-Character.ply";

				// Wireframe, to make it easier to see (for now)
	//			pTestSM->bIsWireFrame = true;
	//			pTestSM->bDontLight = true;

				// Set this mesh to the skinned mesh object
				pTestSM->pSimpleSkinnedMesh = g_pRPGSkinnedMesh;
				// HACK
				//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
	//			pTestSM->currentAnimation = "Idle";

				cAnimationState* pAniState;
				pTestSM->pAniState = new cAnimationState();

				pTestSM->pAniState->defaultAnimation.name = "Idle";
				pTestSM->currentAnimation = "Idle";


				sTextureInfo testObjectTexture;
				testObjectTexture.name = "grass.bmp";
				testObjectTexture.strength = 1.0f;

				pTestSM->vecTextures.push_back(sTextureInfo(testObjectTexture));
				pTestSM->scale = 0.10f;
				vecObjectsToDraw.push_back(pTestSM);
			}
		}//if ( ! AssimpSM_to_VAO_Converter(
	}
	return;

}

void cSceneUtils::generateMaze()
{
	theMM.GenerateMaze(MAZE_WIDTH, MAZE_HEIGHT);
	theMM.PrintMaze();
}

void cSceneUtils::drawMaze(GLuint program)
{
	cMeshObject* cube = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cube");
	cube->isVisible = true;
	cube->dontLight = false;
	cube->isWireFrame = false;

	glm::mat4 matCube(1.0f);

	glm::vec3 positionToDrawNextCube = glm::vec3(0.0f, (CUBE_LENGTH / 2.0f), 0.0f);
	for (unsigned int a = 0; a < this->theMM.maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->theMM.maze[a].size(); b++)
		{
			if (this->theMM.maze[a][b][0])
			{
				cube->position = positionToDrawNextCube;
				drawObject(cube, matCube, program);
			}

			//if (b == this->theMM.maze[a].size() - 1)
			//{
			//	continue;
			//}
			positionToDrawNextCube.z += CUBE_LENGTH;

		}
		positionToDrawNextCube.z -= (CUBE_LENGTH * (MAZE_WIDTH - 1));
		positionToDrawNextCube.x += CUBE_LENGTH;
	}
	cube->isVisible = false;
}

void cSceneUtils::placeDalek()
{
	cMeshObject* dalek = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("dalek");

	unsigned int z = 3;

	for (unsigned int i = 1; i < MAZE_HEIGHT - 1; i++)
	{
		if (!theMM.maze[i][z][0])
		{
			this->dalekLocation[0] = i;
			this->dalekLocation[1] = z;
			break;
		}
	}
	glm::vec3 dalekPosition = glm::vec3(this->dalekLocation[0] * CUBE_LENGTH, 0.0f, this->dalekLocation[1] * CUBE_LENGTH);
	dalek->position = dalekPosition;
}

void cSceneUtils::placePlayer()
{
	cMeshObject* player = (cMeshObject*) findObjectByFriendlyName("SM_Object");

	unsigned int z = 3;

	for (unsigned int i = (MAZE_HEIGHT / 2); i < MAZE_HEIGHT - 1 ; i++)
	{
		if (!theMM.maze[i][z][0])
		{
			this->playerLocation[0] = i;
			this->playerLocation[1] = z;
			break;
		}
	}
	glm::vec3 playerPosition = glm::vec3(this->playerLocation[0] * CUBE_LENGTH, 0.0f, this->playerLocation[1] * CUBE_LENGTH);
	player->position = playerPosition;
	this->playerForward = playerPosition;
	this->playerForward.y = 10.0f;
	this->playerForward.z += CUBE_LENGTH;
}

void cSceneUtils::movePlayerZ(bool forward)
{
	int offset = 1;
	if (!forward)
	{
		offset = -1;
	}
	if (shouldUpdate)
	{
		return;
	}

	if ((this->playerLocation[1] + offset) >= (MAZE_WIDTH - 1))
	{
		return;
	}

	if (theMM.maze[this->playerLocation[0]][this->playerLocation[1] + offset][0])
	{
		return;
	}
	cMeshObject* player = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("SM_Object");
	if (forward)
	{
		player->setOrientationEulerAngles(0.0f, 0.0f, 0.0f, true);
	}
	else
	{
		player->setOrientationEulerAngles(0.0f, 180.0f, 0.0f, true);
	}

	// start animation
	player->currentAnimation = "Unarmed-Walk";
	walkAnimTime = 0.0f;
	this->shouldUpdate = true;

	this->playerLocation[1] += offset;
	glm::vec3 playerPosition = glm::vec3(this->playerLocation[0] * CUBE_LENGTH, 0.0f, this->playerLocation[1] * CUBE_LENGTH);
	nextPlayerPosition = playerPosition;
	nextCameraPosition = cCamera::getInstance()->eye;
	nextCameraPosition.z += (offset * CUBE_LENGTH);
	cameraDirToMove = nextCameraPosition - cCamera::getInstance()->eye;
	if (forward)
	{
		this->playerForward = playerPosition;
		this->playerForward.y = 10.0f;
		this->playerForward.z += CUBE_LENGTH;
	}
	else
	{
		this->playerForward = playerPosition;
		this->playerForward.y = 10.0f;
		this->playerForward.z -= CUBE_LENGTH;
	}
}
void cSceneUtils::movePlayerX(bool right)
{
	if (shouldUpdate)
	{
		return;
	}
	 int offset = 1;
	if (!right)
	{
		offset = -1;
	}

	if ((this->playerLocation[0] + offset) >= (MAZE_HEIGHT - 1))
	{
		return;
	}

	if (theMM.maze[this->playerLocation[0] + offset][this->playerLocation[1]][0])
	{
		return;
	}
	cMeshObject* player = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("SM_Object");
	if (right)
	{
		player->setOrientationEulerAngles(0.0f, 90.0f, 0.0f, true);
	}
	else
	{
		player->setOrientationEulerAngles(0.0f, -90.0f, 0.0f, true);
	}

	// start animation
	player->currentAnimation = "Unarmed-Walk";
	walkAnimTime = 0.0f;
	this->shouldUpdate = true;

	this->playerLocation[0] += offset;
	glm::vec3 playerPosition = glm::vec3(this->playerLocation[0] * CUBE_LENGTH, 0.0f, this->playerLocation[1] * CUBE_LENGTH);
	nextPlayerPosition = playerPosition;
	nextCameraPosition = cCamera::getInstance()->eye;
	nextCameraPosition.x += (offset * CUBE_LENGTH);
	cameraDirToMove = nextCameraPosition - cCamera::getInstance()->eye;

	if (right)
	{
		this->playerForward = playerPosition;
		this->playerForward.y = 10.0f;
		this->playerForward.x += CUBE_LENGTH;
	}
	else
	{
		this->playerForward = playerPosition;
		this->playerForward.y = 10.0f;
		this->playerForward.x -= CUBE_LENGTH;
	}
}

void cSceneUtils::moveCamera()
{

}

void cSceneUtils::selectNextCharacter()
{
	if (selectedCharacterIndex >= characters.size() - 1)
	{
		selectedCharacterIndex = 0;
	}
	else
	{
		selectedCharacterIndex++;
	}
	selectedCharacter = characters[selectedCharacterIndex];
}

void cSceneUtils::cartoonize()
{
	
		for (size_t i = 0; i < vecObjectsToDraw.size(); i++)
		{
			if (this->toon)
			{
				vecObjectsToDraw[i]->dontLight = true;
				if (vecObjectsToDraw[i]->friendlyName == "ground")
				{
					continue;
				}
				vecObjectsToDraw[i]->toon = true;
			}
			else
			{
				vecObjectsToDraw[i]->dontLight = false;
				vecObjectsToDraw[i]->toon = false;
			}

		}
}
