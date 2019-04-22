#include "cEntity.h"



cEntity::cEntity()
{
	this->mesh = NULL;
}


cEntity::~cEntity()
{}

std::string cEntity::getName()
{
	if (this->mesh == NULL)
	{
		return "";
	}
	return this->mesh->friendlyName;
}

cMeshObject* cEntity::GetMesh()
{
	return this->mesh;
}

glm::vec3 cEntity::getPreviousPos()
{
	return glm::vec3(0.0f);
}
