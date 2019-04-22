#pragma once
#include "Behaviour.h"
#include "iEntity.h"

class PersueBehaviour : public Behaviour
{
public:
	PersueBehaviour(iEntity* agent, iEntity* target);
	virtual ~PersueBehaviour();

	virtual void Update(float dt);
private:
	iEntity* mAgent;
	iEntity* mTarget;
};

