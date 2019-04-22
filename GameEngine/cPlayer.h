#pragma once
#include <glm/vec3.hpp>
#include "cMeshObject.h"
#include "iEntity.h"
#include "cCannonBall.h"

class cPlayer : public iEntity
{
public:
	cPlayer();
	~cPlayer();
	cMeshObject* mesh;
	virtual std::string getName();
	virtual cMeshObject* GetMesh();
	virtual glm::vec3 getPreviousPos();

	void move(float amount);
	glm::vec3 at;
	glm::vec3 front;
	std::vector<cCannonBall*> cannonBalls;


	void updateAt();

	void shootCannonBall();
	void update(float dt);
	float mDt;
	glm::vec3 previousPos;
};

