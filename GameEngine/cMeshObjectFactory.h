#ifndef _MESH_OBJECT_FACTORY_
#define _MESH_OBJECT_FACTORY_

#include "iMeshObject.h"
#include "cMeshObject.h"

class cMeshObjectFactory
{
public:
	cMeshObjectFactory();
	~cMeshObjectFactory();
	static iMeshObject* createMeshObject();
};

#endif // !_MESH_OBJECT_FACTORY_



