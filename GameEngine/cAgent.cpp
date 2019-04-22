#include "cAgent.h"



cAgent::cAgent()
{
	this->mesh = NULL;
	this->at = glm::vec3(0.0f, 0.0f, 1.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->previousPos = glm::vec3(0.0f);
}


cAgent::~cAgent()
{}

std::string cAgent::getName()
{
	if (this->mesh == NULL)
	{
		return "";
	}
	return this->mesh->friendlyName;
}

cMeshObject * cAgent::GetMesh()
{
	return this->mesh;
}

glm::vec3 cAgent::getPreviousPos()
{
	return this->previousPos;
}


