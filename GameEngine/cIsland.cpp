#include "cIsland.h"



cIsland::cIsland()
{
	this->randomBaseToDestroy = NULL;
	this->isHitForFirstTime = false;
	this->isHighDestroyed = false;
	this->isLowDestroyed = false;
	this->lost = false;
}


cIsland::~cIsland()
{}

cMeshObject* cIsland::getMesh()
{
	return (cMeshObject*)mesh;
}

void cIsland::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;

}

std::string cIsland::getName()
{
	if (this->mesh != NULL)
	{
		return this->getMesh()->friendlyName;
	}
	return "";
}