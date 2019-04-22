#pragma once
#include "iEntity.h"
#include "cCannonBall.h"

class cAgent : public iEntity
{
public:
	cAgent();
	~cAgent();
	cMeshObject* mesh;
	virtual std::string getName();
	virtual cMeshObject* GetMesh();
	virtual glm::vec3 getPreviousPos();
	glm::vec3 at;
	glm::vec3 front;
	glm::vec3 previousPos;
	cCannonBall* cannonBall;
};

