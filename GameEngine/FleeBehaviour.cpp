#include "FleeBehaviour.h"
#include <iostream>


FleeBehaviour::FleeBehaviour(iEntity* agent, iEntity* target)
{
	this->mAgent = agent;
	this->mTarget = target;
}


FleeBehaviour::~FleeBehaviour()
{

}

void FleeBehaviour::Update(float dt)
{
	glm::vec3 forward = mAgent->GetMesh()->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 seek = glm::normalize(mTarget->GetMesh()->position - mAgent->GetMesh()->position);

	glm::quat orientation = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));

	float angle = glm::degrees(glm::acos(glm::dot(orientation, mTarget->GetMesh()->getOrientation())));
	mAgent->GetMesh()->materialDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	if ((angle >= 0.0f && angle <= 10.0f) || (angle <= 180.0f && angle >= 170.0f))
	{
		seek = -seek;
		orientation = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));
		mAgent->GetMesh()->materialDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	mAgent->GetMesh()->setOrientation(orientation);

	mAgent->GetMesh()->position.x += seek.x * 0.2f;
	mAgent->GetMesh()->position.z += seek.z * 0.2f;
}
