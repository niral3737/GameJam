#include "cCannonBall.h"

cCannonBall::cCannonBall()
{
	this->isShot = false;
	this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
}


cCannonBall::~cCannonBall()
{}

cMeshObject * cCannonBall::GetMesh()
{
	return this->mesh;
}


std::string cCannonBall::getName()
{
	if (this->mesh != NULL)
	{
		return this->mesh->friendlyName;
	}
	return "";
}

glm::vec3 cCannonBall::getPreviousPos()
{
	return this->previousPos;
}