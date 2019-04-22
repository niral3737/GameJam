#pragma once
class Behaviour
{
public:
	Behaviour(){}
	virtual ~Behaviour() {}

	virtual void Update(float dt) = 0;
};

