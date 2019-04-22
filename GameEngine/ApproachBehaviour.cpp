#include "ApproachBehaviour.h"
#include "cAgent.h"


ApproachBehaviour::ApproachBehaviour(iEntity* agent, iEntity* target)
{
	this->mAgent = agent;
	this->mTarget = target;
}


ApproachBehaviour::~ApproachBehaviour()
{}

void ApproachBehaviour::Update(float dt)
{
	glm::vec3 forward = mAgent->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 seek = glm::normalize(mTarget->GetMesh()->position - mAgent->GetMesh()->position);

	glm::quat orientation = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));

	mAgent->GetMesh()->setOrientation(orientation);

	float distance = glm::distance(mAgent->GetMesh()->position, mTarget->GetMesh()->position);
	
	cAgent* agent = (cAgent*)mAgent;
	if (distance > 50.0f)
	{
		mAgent->GetMesh()->position.x += seek.x * 0.2f;
		mAgent->GetMesh()->position.z += seek.z * 0.2f;
		agent->cannonBall->mesh->position = mAgent->GetMesh()->position;
	}
	else
	{
		agent->cannonBall->mesh->position.x += seek.x * 0.4f;
		agent->cannonBall->mesh->position.z += seek.z * 0.4f;

		float distanceFromTarget = glm::distance(mAgent->GetMesh()->position, mTarget->GetMesh()->position);
		float distance = glm::distance(agent->cannonBall->mesh->position, mAgent->GetMesh()->position);
		if (distance >= distanceFromTarget)
		{
			agent->cannonBall->mesh->position = mAgent->GetMesh()->position;
		}

	}
	
}