#include "cMeshObject.h"


unsigned int cMeshObject::mNextID = cMeshObject::STARTING_ID_VALUE;

cMeshObject::cMeshObject()
{
	this->position = glm::vec3(0.0f);
	this->postRotation = glm::vec3(0.0f);
	//this->objectColor = glm::vec3(1.0f);
	this->materialDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	this->scale = 1.0f;

	this->isVisible = true;
	this->isWireFrame = false;
	this->useVertexColor = false;
	this->dontLight = false;

	this->isUpdatedByPhysics = true;
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	
	this->radius = 0.0f;

	this->mUniqueID = cMeshObject::mNextID;
	cMeshObject::mNextID++;

	this->rigidBody = NULL;

	this->isReflective = false;
	this->isRefractive = false;

	this->toon = false;
	return;
}

cMeshObject::~cMeshObject()
{}

glm::quat cMeshObject::getOrientation(void)
{
	return this->orientation;
}
void cMeshObject::setOrientation(glm::quat orientation)
{
	this->orientation = orientation;
}

void cMeshObject::setOrientationEulerAngles(glm::vec3 newEulerAngles, bool bIsDegrees)
{
	if (bIsDegrees)
	{
		newEulerAngles = glm::vec3(glm::radians(newEulerAngles.x),
			glm::radians(newEulerAngles.y),
			glm::radians(newEulerAngles.z));
	}

	this->orientation = glm::quat(glm::vec3(newEulerAngles.x, newEulerAngles.y, newEulerAngles.z));
}

void cMeshObject::setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees)
{
	this->setOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cMeshObject::adjustOrientationEulerAngles(glm::vec3 adjEulerAngle, bool bIsDegrees)
{
	if (bIsDegrees)
	{
		adjEulerAngle = glm::vec3(glm::radians(adjEulerAngle.x),
			glm::radians(adjEulerAngle.y),
			glm::radians(adjEulerAngle.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjEulerAngle);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	//this->orientation *= rotationAdjust;
	adjustOrientation(rotationAdjust);
}

void cMeshObject::adjustOrientationEulerAngles(float x, float y, float z, bool bIsDegrees)
{
	this->adjustOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cMeshObject::adjustOrientation(glm::quat adjOrient)
{
	this->orientation *= adjOrient;
}

iMeshObject* cMeshObject::create(void)
{
	return new cMeshObject();
}

cMeshObject* cMeshObject::cloneObject()
{
	cMeshObject* clone = new cMeshObject();

	clone->position = this->position;
	clone->postRotation = this->postRotation;
	clone->materialDiffuse = this->materialDiffuse;
	clone->materialSpecular = this->materialSpecular;
	clone->scale = this->scale;
	clone->meshName = this->meshName;
	clone->friendlyName = this->friendlyName;
	clone->isWireFrame = this->isWireFrame;
	clone->isVisible = this->isVisible;
	clone->useVertexColor = this->useVertexColor;
	clone->dontLight = this->dontLight;
	clone->isUpdatedByPhysics = this->isUpdatedByPhysics;
	clone->velocity = this->velocity;
	clone->acceleration = this->acceleration;
	clone->orientation = this->orientation;

	clone->radius = this->radius;


	return clone;
}

void cMeshObject::setDiffuseColour(glm::vec3 newDiffuse)
{
	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
	return;
}

void cMeshObject::setAlphaTransparency(float newAlpha)
{
	this->materialDiffuse.a = newAlpha;
	return;
}

void cMeshObject::setSpecularColour(glm::vec3 colourRGB)
{
	this->materialSpecular = glm::vec4(colourRGB, this->materialSpecular.a);
	return;
}

void cMeshObject::setSpecularPower(float specPower)
{
	this->materialSpecular.a = specPower;
	return;
}

float cMeshObject::getSpecularPower()
{
	return this->materialSpecular.a;
}
