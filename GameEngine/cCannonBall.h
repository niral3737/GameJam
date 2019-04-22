#ifndef _CANNON_BALL_HG_
#define _CANNON_BALL_HG_

#include "iEntity.h"
#include "cMeshObject.h"

class cCannonBall :
	public iEntity
{
public:
	cCannonBall();
	virtual ~cCannonBall();
	virtual cMeshObject* GetMesh();
	virtual glm::vec3 getPreviousPos();
	bool isShot;

	cMeshObject* mesh;
	glm::vec3 direction;
	virtual std::string getName();
	glm::vec3 previousPos;
};


#endif // !_CANNON_BALL_HG_


