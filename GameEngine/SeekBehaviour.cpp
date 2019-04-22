#include "SeekBehaviour.h"



SeekBehaviour::SeekBehaviour(iEntity* agent, iEntity* target)
{
	this->mAgent = agent;
	this->mTarget = target;
}


SeekBehaviour::~SeekBehaviour()
{}

void SeekBehaviour::Update(float dt)
{
	glm::vec3 forward = mAgent->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 seek = glm::normalize(mTarget->GetMesh()->position - mAgent->GetMesh()->position);

	glm::quat orientation = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));

	mAgent->GetMesh()->setOrientation(orientation);

	mAgent->GetMesh()->position.x += seek.x * 0.2f;
	mAgent->GetMesh()->position.z += seek.z * 0.2f;
}
