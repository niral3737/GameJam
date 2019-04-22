#include "EvadeBehaviour.h"
#include "cCannonBall.h"


EvadeBehaviour::EvadeBehaviour(iEntity* agent, iEntity* target)
{
	this->mAgent = agent;
	this->mTarget = target;
}


EvadeBehaviour::~EvadeBehaviour()
{}

void EvadeBehaviour::Update(float dt)
{
	cCannonBall* cannonBall = dynamic_cast<cCannonBall*> (mTarget);
	if (cannonBall && !cannonBall->isShot)
	{
		return;
	}
	float distance = glm::distance(mAgent->GetMesh()->position, mTarget->GetMesh()->position);

	if (distance > 100.0f)
	{
		return;
	}
	glm::vec3 forward = mAgent->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 targetForward = mTarget->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 velocity = mTarget->GetMesh()->position - mTarget->getPreviousPos();
	if (!(velocity.x <= 0.00001f && velocity.z <= 0.00001f))
	{
		velocity *= 50.0f;
	}
	glm::vec3 targetFuturePos = mTarget->GetMesh()->position + (targetForward) * (mTarget->GetMesh()->position - mTarget->getPreviousPos()) * 50.0f;

	glm::vec3 seek = glm::normalize(targetFuturePos - mAgent->GetMesh()->position);
	seek = -seek;
	glm::quat orientation = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));

	mAgent->GetMesh()->setOrientation(orientation);

	mAgent->GetMesh()->position.x += seek.x * 0.2f;
	mAgent->GetMesh()->position.z += seek.z * 0.2f;
}
