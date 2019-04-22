#pragma once
#include "Behaviour.h"
#include "iEntity.h"

class WanderAndIdleBehaviour : public Behaviour
{
public:
	WanderAndIdleBehaviour(iEntity* agent);
	virtual ~WanderAndIdleBehaviour();

	virtual void Update(float dt);
	void chooseRandomLocationInBounds();
	void initialize();
private:
	iEntity* mAgent;
	glm::vec3 currentLocationToGo;
	glm::vec3 direction;
	float waitTime;
	bool isIdle;
	float timeWaited;
};

