#include "cJet.h"

#include "cSceneUtils.h"

cJet::cJet()
{
	this->mesh = NULL;
	this->collisionPointRadius = 3.0f;
	this->showCollisionPoints = false;
}


cJet::~cJet()
{}

cMeshObject* cJet::getMesh()
{
	return (cMeshObject*)mesh;
}

void cJet::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;

}

std::string cJet::getName()
{
	if (this->mesh != NULL)
	{
		return this->getMesh()->friendlyName;
	}
	return "";
}

void cJet::drawCollisionPoints(GLuint shaderProgramID)
{
	if (!this->showCollisionPoints)
	{
		return;
	}
	for (size_t i = 0; i < this->collisionPointsWorld.size(); i++)
	{
		cMeshObject* sphere = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("sphere");
		sphere->isVisible = true;
		sphere->isWireFrame = true;
		sphere->dontLight = true;
		//The actual size of the sphere is 8 radius so to get the collisionRadius size sphere we're deviding it by 8.0f
		sphere->scale = this->collisionPointRadius / 8.0f;

		glm::mat4 matIdentity = glm::mat4(1.0f);

		sphere->materialDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		sphere->position = collisionPointsWorld[i];
		cSceneUtils::getInstance()->drawObject(sphere, matIdentity, shaderProgramID);
		sphere->isVisible = false;
	}
}

void cJet::applyTransformationsToCollisionPoints()
{
	glm::mat4 matTransform = glm::mat4(1.0f);	// identity matrix

	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
		this->getMesh()->position);

	matTransform = matTransform * matTranslation;

	// Now orientation
	glm::quat qRotation = this->getMesh()->getOrientation();
	// Make a mat4x4 from that quaternion
	glm::mat4 matQrotation = glm::mat4(qRotation);

	matTransform = matTransform * matQrotation;

	this->collisionPointsWorld.clear();
	for (size_t i = 0; i < this->collisionPointsModel.size(); i++)
	{
		this->collisionPointsWorld.push_back(matTransform * collisionPointsModel[i]);
	}
}

void cJet::showCollisionPoint(unsigned int pointIndex, GLuint shaderProgramID)
{
	if (!this->showCollisionPoints)
	{
		return;
	}

	cMeshObject* sphere = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("sphere");
	sphere->isVisible = true;
	sphere->isWireFrame = true;
	sphere->dontLight = true;
	//The actual size of the sphere is 8 radius so to get the collisionRadius size sphere we're deviding it by 8.0f
	sphere->scale = this->collisionPointRadius / 8.0f;

	glm::mat4 matIdentity = glm::mat4(1.0f);

	sphere->materialDiffuse = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	sphere->position = collisionPointsWorld[pointIndex];
	cSceneUtils::getInstance()->drawObject(sphere, matIdentity, shaderProgramID);
	sphere->isVisible = false;
}
