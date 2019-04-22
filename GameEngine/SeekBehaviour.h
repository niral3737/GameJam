#pragma once
#include "Behaviour.h"
#include "iEntity.h"

class SeekBehaviour : public Behaviour
{
public:
	SeekBehaviour(iEntity* agent, iEntity* target);
	virtual ~SeekBehaviour();

	virtual void Update(float dt);

private:
	iEntity* mAgent;
	iEntity* mTarget;
};

