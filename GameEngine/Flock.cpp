#include "Flock.h"
#include <iostream>

Flock::Flock(void)
	: cohesionWeight(1.0f / 3.0f)
	, separationWeight(1.0f / 3.0f)
	, alignmentWeight(1.0f / 3.0f)
	, centerPoint(glm::vec3(0.0f))
	, radius(600.0f)
{

}

Flock::Flock(float cohesionWeight, float separationWeight, float alignmentWeight)
	: cohesionWeight(cohesionWeight)
	, separationWeight(separationWeight)
	, alignmentWeight(alignmentWeight)
	, centerPoint(glm::vec3(0.0f))
	, radius(600.0f)
{

}

Flock::~Flock(void)
{
}


void Flock::AddFlockMember(cEntity* entity)
{
	FlockMemberInfo* flockMemberInfo = new FlockMemberInfo(entity);
	mFlockMembers.push_back(flockMemberInfo);
}

void Flock::RemoveFlockMember(cEntity* entity)
{
}

void Flock::CalculateSteering(void)
{
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		glm::vec3 alignment = calculateAlignment(*itMember);
		glm::vec3 cohesion = calculateCohesion(*itMember);
		glm::vec3 separation = calculateSeparation(*itMember);

		(*itMember)->entity->GetMesh()->velocity += (alignment * alignmentWeight + cohesion * cohesionWeight + separation * separationWeight);
		(*itMember)->entity->GetMesh()->setOrientation(glm::quatLookAt(-glm::normalize((*itMember)->entity->GetMesh()->velocity), glm::vec3(0.0f, 1.0f, 0.0f)));
		(*itMember)->entity->GetMesh()->velocity = glm::normalize((*itMember)->entity->GetMesh()->velocity) * 20.0f;
		(*itMember)->entity->GetMesh()->velocity.y = 0.0f;
	}
}

void Flock::addCohesion(float delCohesion)
{
	if (cohesionWeight + delCohesion <= 0.0f)
	{
		cohesionWeight = 0.0f;
	}
	else
	{
		cohesionWeight += delCohesion;
	}
	std::cout << "cohesion: " << cohesionWeight << " separation: " << separationWeight << " alignment: " << alignmentWeight << std::endl;
}

void Flock::addAllignment(float delAllignment)
{
	if (alignmentWeight + delAllignment <= 0.0f)
	{
		alignmentWeight = 0.0f;
	}
	else
	{
		alignmentWeight += delAllignment;
	}
	std::cout << "cohesion: " << cohesionWeight << " separation: " << separationWeight << " alignment: " << alignmentWeight << std::endl;
}

void Flock::addSeparation(float delSeparation)
{
	if (separationWeight + delSeparation <= 0.0f)
	{
		separationWeight = 0.0f;
	}
	else
	{
		separationWeight += delSeparation;
	}
	std::cout << "cohesion: " << cohesionWeight << " separation: " << separationWeight << " alignment: " << alignmentWeight << std::endl;
}

Flock::FlockMemberInfo::FlockMemberInfo(cEntity* entity)
	: entity(entity)
{

}
Flock::FlockMemberInfo::~FlockMemberInfo(void)
{}

glm::vec3 Flock::calculateAlignment(FlockMemberInfo * member)
{
	glm::vec3 alignment = glm::vec3(0.0f);
	unsigned int neighbourCount = 0;
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		if (!((*itMember)->entity->GetMesh()->friendlyName == member->entity->GetMesh()->friendlyName))
		{
			float distance = glm::distance((*itMember)->entity->GetMesh()->position, member->entity->GetMesh()->position);

			if (distance < radius)
			{
				alignment += (*itMember)->entity->GetMesh()->velocity;
				neighbourCount++;
			}
			else
			{
				(*itMember)->entity->GetMesh()->position = centerPoint;
			}
		}
	}
	if (neighbourCount == 0)
	{
		return alignment;
	}

	alignment /= neighbourCount;
	return glm::normalize(alignment);
}

glm::vec3 Flock::calculateCohesion(FlockMemberInfo * member)
{
	glm::vec3 cohesion = glm::vec3(0.0f);
	unsigned int neighbourCount = 0;
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		if (!((*itMember)->entity->GetMesh()->friendlyName == member->entity->GetMesh()->friendlyName))
		{
			float distance = glm::distance((*itMember)->entity->GetMesh()->position, member->entity->GetMesh()->position);
			if (distance < radius)
			{
				cohesion += (*itMember)->entity->GetMesh()->position;
				neighbourCount++;
			}
			else
			{
				(*itMember)->entity->GetMesh()->position = centerPoint;
			}
		}
	}
	//add centerPoint 100 times
	cohesion += (centerPoint * 100.0f);
	neighbourCount += 100;
	if (neighbourCount == 0)
	{
		return cohesion;
	}

	cohesion /= neighbourCount;
	cohesion = cohesion - member->entity->GetMesh()->position;

	return glm::normalize(cohesion);
}

glm::vec3 Flock::calculateSeparation(FlockMemberInfo * member)
{
	glm::vec3 separation = glm::vec3(0.0f);
	unsigned int neighbourCount = 0;
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		if (!((*itMember)->entity->GetMesh()->friendlyName == member->entity->GetMesh()->friendlyName))
		{
			float distance = glm::distance((*itMember)->entity->GetMesh()->position, member->entity->GetMesh()->position);
			if (distance < radius)
			{
				if (distance != 0.0f)
				{
					separation += ((*itMember)->entity->GetMesh()->position - member->entity->GetMesh()->position);
					neighbourCount++;
				}
				
			}
			else
			{
				(*itMember)->entity->GetMesh()->position = centerPoint;
			}
		}
	}

	//neighbourCount += 30;
	if (neighbourCount == 0)
	{
		return separation;
	}

	separation /= neighbourCount;
	separation = separation - member->entity->GetMesh()->position;
	separation = -separation;
	return glm::normalize(separation);
}
