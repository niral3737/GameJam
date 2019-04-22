#ifndef _ISLAND_HG_
#define _ISLAND_HG_

#include "iEntity.h"
#include "iMeshObject.h" 
#include "cMeshObject.h"
#include "cBase.h"
#include "cCannonBall.h"
#include <vector>

class cIsland : public iEntity
{
public:
	cIsland();
	virtual ~cIsland();

	//first 3 are high bases and others are low bases
	std::vector<cBase*> bases;
	cCannonBall* highCannnonBall;
	cCannonBall* lowCannnonBall;

	bool isHitForFirstTime;

	cBase* randomBaseToDestroy;
	cIsland* islandOnWhichRandomBaseIs;
	glm::vec3 directionToShootIn;
	float upVelocity;
	bool isHighDestroyed;
	bool isLowDestroyed;
	bool lost;

	iMeshObject* mesh;
	virtual std::string getName();
	cMeshObject* getMesh();
	void setMesh(iMeshObject* mesh);
};


#endif // !_ISLAND_HG_



