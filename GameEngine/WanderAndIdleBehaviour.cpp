#include "WanderAndIdleBehaviour.h"
#include "cRandomHelper.h"


WanderAndIdleBehaviour::WanderAndIdleBehaviour(iEntity* agent)
{
	this->mAgent = agent;
	this->currentLocationToGo = glm::vec3(0.0f);
	chooseRandomLocationInBounds();
	this->waitTime = 3.0f; 
	this->isIdle = false;
	this->timeWaited = 0.0f;
	this->direction = glm::normalize(currentLocationToGo - agent->GetMesh()->position);
}

WanderAndIdleBehaviour::~WanderAndIdleBehaviour()
{}

void WanderAndIdleBehaviour::Update(float dt)
{
	if (this->isIdle)
	{
		mAgent->GetMesh()->materialDiffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		this->timeWaited += dt;
		if (this->timeWaited > this->waitTime)
		{
			this->isIdle = false;
		}
		return;
	}
	mAgent->GetMesh()->materialDiffuse = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	float distance = glm::distance(currentLocationToGo, mAgent->GetMesh()->position);

	if (distance <=2.0f)
	{
		mAgent->GetMesh()->position = currentLocationToGo;
		initialize();
	}
	else
	{
		mAgent->GetMesh()->position.x += direction.x;
		mAgent->GetMesh()->position.z += direction.z;
	}

}

void WanderAndIdleBehaviour::chooseRandomLocationInBounds()
{
	currentLocationToGo.x = cRandomHelper::generateRandomfloatInRange(-120.0f, 600.0f);
	currentLocationToGo.z = cRandomHelper::generateRandomfloatInRange(-660.0f, -330.0f);
}

void WanderAndIdleBehaviour::initialize()
{
	this->isIdle = true;
	this->timeWaited = 0.0f;
	chooseRandomLocationInBounds();
	this->direction = glm::normalize(currentLocationToGo - mAgent->GetMesh()->position);
}