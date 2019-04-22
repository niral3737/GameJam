#pragma once
#include "Behaviour.h"
#include "iEntity.h"
class EvadeBehaviour : public Behaviour
{
public:
	EvadeBehaviour(iEntity* agent, iEntity* target);
	~EvadeBehaviour();

	virtual void Update(float dt);
private:
	iEntity* mAgent;
	iEntity* mTarget;
};

