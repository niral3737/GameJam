#include "cMeshObjectFactory.h"



cMeshObjectFactory::cMeshObjectFactory()
{}


cMeshObjectFactory::~cMeshObjectFactory()
{}


iMeshObject* cMeshObjectFactory::createMeshObject()
{
	return cMeshObject::create();
}