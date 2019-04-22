#ifndef _MESH_OBJECT_HG_
#define _MESH_OBJECT_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include <string>
#include <vector>

#include "iMeshObject.h"
#include <physics/interfaces/iRigidBody.h>

// This is for the skinned mesh and animation
class cSimpleAssimpSkinnedMesh;	// Forward declare
class cAnimationState;			// Forward declare 

struct sTextureInfo
{
	std::string name;
	int cachedTextureID; 	// Set to -1 by default
	float strength;		// Set to 0.0f by default
};

struct sLODInfo
{
	sLODInfo() :
		minDistance(FLT_MAX)
	{}
	std::string meshName;	// Model to draw
	sLODInfo(std::string meshName_)
	{
		this->meshName = meshName_;
		this->minDistance = FLT_MAX;
	};
	sLODInfo(std::string meshName_, float minDistanceToDraw)
	{
		this->meshName = meshName_;
		this->minDistance = minDistanceToDraw;
	};

	// If it's at or closer than this, draw this model
	float minDistance;
};


class cMeshObject : public iMeshObject
{
public:
	cMeshObject();
	~cMeshObject();

	glm::vec3 position;
	glm::vec3 postRotation;
	glm::vec4 materialDiffuse;
	void setDiffuseColour(glm::vec3 newDiffuse);
	void setAlphaTransparency(float newAlpha);
	glm::vec4 materialSpecular;
	void setSpecularColour(glm::vec3 colourRGB);
	void setSpecularPower(float specPower);
	float getSpecularPower();
	float scale;

	std::string meshName;
	std::string friendlyName;

	bool isWireFrame;
	bool isVisible;
	bool useVertexColor;
	bool dontLight;

	float radius;

	//Physics
	bool isUpdatedByPhysics;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::quat getOrientation(void);
	void setOrientation(glm::quat orientation);
	void setOrientationEulerAngles(glm::vec3 newEulerAngles, bool bIsDegrees = false);
	void setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjustOrientationEulerAngles(glm::vec3 adjEulerAngle, bool bIsDegrees = false);
	void adjustOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjustOrientation(glm::quat adjOrient);

	inline unsigned int getUniqueId(void)
	{
		return this->mUniqueID;
	}

	static iMeshObject* create(void);
	cMeshObject* cloneObject();

	// Textures...
	std::vector<sTextureInfo> vecTextures;

	// Skinned mesh and animations:
	// If NULL, then object ISN'T a skinned mesh
	cSimpleAssimpSkinnedMesh*	pSimpleSkinnedMesh;
	// HACK
	std::string currentAnimation;
	cAnimationState*	pAniState;

	// Extent Values for skinned mesh
	// These can be updated per frame, from the "update skinned mesh" call
	glm::vec3 minXYZ_from_SM_Bones;
	glm::vec3 maxXYZ_from_SM_Bones;
	// Store all the bones for this model, being updated per frame
	std::vector< glm::mat4x4 > vecObjectBoneTransformation;

	nPhysics::iRigidBody* rigidBody;
	//HACK: This will be replaced with something more sophisticated, later
	// For now, there is only one off screen FBO
	bool b_HACK_UsesOffscreenFBO;

	bool isReflective;
	bool isRefractive;

	std::vector<sLODInfo> vecLODMeshs;

	bool toon;
private:
	
	unsigned int mUniqueID;			// Number that's unique to this instance
	static unsigned int mNextID; 

	static const unsigned int STARTING_ID_VALUE = 1000;

	glm::quat orientation;
};

#endif // !_MESH_OBJECT_HG_



