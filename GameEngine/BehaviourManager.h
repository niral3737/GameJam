#pragma once
#include <map>
#include "iEntity.h"
#include "Behaviour.h" 

class BehaviourManager
{
public:
	BehaviourManager();
	~BehaviourManager();

	void SetBehaviour(iEntity* agent, Behaviour* behaviour);
	void RemoveAgent(iEntity* agent);

	void Update(float dt);
private:
	std::map<iEntity*, Behaviour*> mBehaviourMap;
};

typedef std::map<iEntity*, Behaviour*>::iterator behaviour_iterator;

