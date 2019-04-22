#include "cBase.h"



cBase::cBase()
{
	this->health = 100;
	this->isDestroyed = false;
}


cBase::~cBase()
{}

cMeshObject* cBase::getMesh()
{
	return (cMeshObject*)mesh;
}

void cBase::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;

}

std::string cBase::getName()
{
	if (this->mesh != NULL)
	{
		return this->getMesh()->friendlyName;
	}
	return "";
}