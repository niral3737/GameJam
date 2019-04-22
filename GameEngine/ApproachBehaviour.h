#pragma once
#include "Behaviour.h"
#include "iEntity.h"

class ApproachBehaviour : public Behaviour
{
public:
	ApproachBehaviour(iEntity* agent, iEntity* target);
	virtual ~ApproachBehaviour();

	virtual void Update(float dt);

private:
	iEntity* mAgent;
	iEntity* mTarget;
};

