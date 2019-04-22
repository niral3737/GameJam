#pragma once
#include "Behaviour.h"
#include "iEntity.h"

class FleeBehaviour : public Behaviour
{
public:
	FleeBehaviour(iEntity* agent, iEntity* target);
	virtual ~FleeBehaviour();

	virtual void Update(float dt);
private:
	iEntity* mAgent;
	iEntity* mTarget;
};

