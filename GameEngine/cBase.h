#ifndef _BASE_HG_
#define _BASE_HG_

#include "iEntity.h"
#include "iMeshObject.h" 
#include "cMeshObject.h"

class cBase :
	public iEntity
{
public:
	cBase();
	virtual ~cBase();
	unsigned int health;
	bool isDestroyed;
	glm::vec3 hittingVelocity;

	iMeshObject* mesh;
	virtual std::string getName();
	cMeshObject* getMesh();
	void setMesh(iMeshObject* mesh);
};

#endif // !_BASE_HG_



