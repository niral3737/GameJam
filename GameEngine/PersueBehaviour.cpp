#include "PersueBehaviour.h"
#include <iostream>	


PersueBehaviour::PersueBehaviour(iEntity* agent, iEntity* target)
{
	this->mAgent = agent;
	this->mTarget = target;
}

PersueBehaviour::~PersueBehaviour()
{
}

void PersueBehaviour::Update(float dt)
{
	glm::vec3 forward = mAgent->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 targetForward = mTarget->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 velocity = mTarget->GetMesh()->position - mTarget->getPreviousPos();
	if (!(velocity.x <= 0.00001f && velocity.z <= 0.00001f))
	{
		velocity *= 50.0f;
	}
	glm::vec3 targetFuturePos = mTarget->GetMesh()->position + (targetForward) * (mTarget->GetMesh()->position - mTarget->getPreviousPos()) * 50.0f;
	
	glm::vec3 seek = glm::normalize(targetFuturePos - mAgent->GetMesh()->position);

	glm::quat orientation = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));

	mAgent->GetMesh()->setOrientation(orientation);

	mAgent->GetMesh()->position.x += seek.x * 0.2f;
	mAgent->GetMesh()->position.z += seek.z * 0.2f;
}
