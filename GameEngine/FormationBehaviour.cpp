#include "FormationBehaviour.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <math.h>

FormationBehaviour::FormationBehaviour()
{
	mShape = CIRCLE;
	isReadyForNext = false;
	currentShape = 0;
}

FormationBehaviour::~FormationBehaviour()
{}

void FormationBehaviour::addMember(glm::vec3 initPos)
{
	newPositions.push_back(initPos);
	reached.push_back(false);
}

void FormationBehaviour::setFormationShape(eFormationShape shape)
{
	this->mShape = shape;

	updateNewPositions();
}

void FormationBehaviour::nextFormationShape()
{
	if (currentShape >= 3)
	{
		currentShape = 0;
	}
	else
	{
		currentShape++;
	}

	switch (currentShape)
	{
	case 0:
		mShape = eFormationShape::CIRCLE;
		break;
	case 1:
		mShape = eFormationShape::LINE;
		break;
	case 2:
		mShape = eFormationShape::TWO_ROWS;
		break;
	case 3:
		mShape = eFormationShape::V;
		break;
	default:
		mShape = eFormationShape::V;
		break;
	}
}

void FormationBehaviour::updateNewPositions()
{
	size_t numMembers = newPositions.size();
	if (numMembers == 0)
	{
		return;
	}
	
	if (mShape == FormationBehaviour::V)
	{
		int multiplier = 1;
		newPositions[0] = centerPosition;
		for (size_t i = 1; i < numMembers; i++)
		{
			if (i % 2 == 0)//left
			{
				newPositions[i] = centerPosition;
				newPositions[i].x += (DISTANCE_OFFSET * multiplier);
				newPositions[i].z += (DISTANCE_OFFSET * multiplier);
				multiplier++;
			}
			else //right
			{
				newPositions[i] = centerPosition;
				newPositions[i].x -= (DISTANCE_OFFSET * multiplier);
				newPositions[i].z += (DISTANCE_OFFSET * multiplier);
			}
			//glm::vec3 radius = newPositions[i] - centerPosition;
			////float angle = glm::acos(glm::dot(glm::normalize(faceDirection), glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))));
			//float angle = glm::angle(glm::normalize(faceDirection), glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
			//newPositions[i] = glm::rotateY(radius, angle) + centerPosition;
		}
	}
	if (mShape == FormationBehaviour::CIRCLE)
	{
		float const PI = glm::pi<float>();
		
		float rotation = (2.0f * PI) / numMembers;
		for (size_t i = 0; i < numMembers; i++)
		{
			float appliedRotation = i * rotation;
			newPositions[i] = centerPosition;
			newPositions[i].x = centerPosition.x + glm::cos(appliedRotation) * DISTANCE_OFFSET * 10.0f;
			newPositions[i].z = centerPosition.z + glm::sin(appliedRotation) * DISTANCE_OFFSET * 10.0f;
		}
	}
	//if (mShape == FormationBehaviour::SQUARE)
	//{
	//	if (numMembers < 4)
	//	{
	//		return;
	//	}
	//	int remainingMembers = numMembers - 4;
	//	float membersPerLine = ceil(((float)remainingMembers) / 4.0f);
	//	float trueDistanceOffset = (0.5f + (membersPerLine / 2.0f)) * DISTANCE_OFFSET;

	//	four corners
	//	newPositions[0] = glm::vec3(centerPosition.x + trueDistanceOffset, centerPosition.y, centerPosition.z + trueDistanceOffset);
	//	newPositions[1] = glm::vec3(centerPosition.x - trueDistanceOffset, centerPosition.y, centerPosition.z + trueDistanceOffset);
	//	newPositions[2] = glm::vec3(centerPosition.x - trueDistanceOffset, centerPosition.y, centerPosition.z - trueDistanceOffset);
	//	newPositions[3] = glm::vec3(centerPosition.x + trueDistanceOffset, centerPosition.y, centerPosition.z - trueDistanceOffset);

	//	upper side
	//	if (remainingMembers == 0)
	//	{
	//		return;
	//	}
	//	float appliedOffset = -DISTANCE_OFFSET;
	//	for (size_t i = 4; i < (size_t) membersPerLine + 4; i++)
	//	{
	//		appliedOffset *= i - 3;
	//		newPositions[i] = glm::vec3(newPositions[0].x + appliedOffset, newPositions[0].y, newPositions[0].z);
	//	}
	//	remainingMembers -= (size_t)membersPerLine;
	//	if (remainingMembers == 0)
	//	{
	//		return;
	//	}
	//	appliedOffset = -DISTANCE_OFFSET;
	//	for (size_t i = 4 + (size_t)membersPerLine; i < (size_t)membersPerLine * 2 + 4; i++)
	//	{
	//		appliedOffset *= i - 3 - (size_t)membersPerLine;
	//		newPositions[i] = glm::vec3(newPositions[1].x, newPositions[1].y, newPositions[1].z + appliedOffset);
	//	}
	//	remainingMembers -= (size_t)membersPerLine;
	//	if (remainingMembers == 0)
	//	{
	//		return;
	//	}
	//	appliedOffset = DISTANCE_OFFSET;
	//	for (size_t i = 4 + (size_t)membersPerLine * 2; i < (size_t)membersPerLine * 3 + 4; i++)
	//	{
	//		appliedOffset *= i - 3 - (size_t)membersPerLine * 2;
	//		newPositions[i] = glm::vec3(newPositions[2].x + appliedOffset, newPositions[2].y, newPositions[2].z);
	//	}
	//	remainingMembers -= (size_t)membersPerLine;
	//	if (remainingMembers == 0)
	//	{
	//		return;
	//	}
	//	appliedOffset = DISTANCE_OFFSET;
	//	for (size_t i = 4 + (size_t)membersPerLine * 3; i < (size_t)membersPerLine * 4 + 4; i++)
	//	{
	//		appliedOffset *= i - 3 - (size_t)membersPerLine * 3;
	//		newPositions[i] = glm::vec3(newPositions[3].x, newPositions[3].y, newPositions[3].z + appliedOffset);
	//	}



	//	/*float offsetX = DISTANCE_OFFSET;
	//	float offsetZ = DISTANCE_OFFSET;
	//	size_t numLineMembers = (size_t) membersPerLine;
	//	glm::vec3 offsetPosition = newPositions[0];*/
	//	/*for (size_t i = 4; i < remainingMembers; i++)
	//	{
	//		offsetX = 0.0f;
	//		for (size_t j = 0; j < numLineMembers; j++)
	//		{
	//			newPositions[i]
	//		}
	//	}*/
	//}
	if (mShape == FormationBehaviour::LINE)
	{
		int multiplier = 1;
		newPositions[0] = centerPosition;
		for (size_t i = 1; i < numMembers; i++)
		{
			if (i % 2 == 0)//left
			{
				newPositions[i] = centerPosition;
				newPositions[i].x += (DISTANCE_OFFSET * multiplier);
				multiplier++;
			}
			else //right
			{
				newPositions[i] = centerPosition;
				newPositions[i].x -= (DISTANCE_OFFSET * multiplier);
			}
		}
	}
	if (mShape == FormationBehaviour::TWO_ROWS)
	{
		int multiplier = 0;
		int multiplier2 = 0;
		for (size_t i = 0; i < numMembers; i++)
		{
			if (i < (numMembers / 2))
			{
				if (i % 2 == 0)
				{
					newPositions[i] = centerPosition;
					newPositions[i].x += (DISTANCE_OFFSET * multiplier);
				}
				else
				{
					multiplier++;
					newPositions[i] = centerPosition;
					newPositions[i].x -= (DISTANCE_OFFSET * multiplier);
				}
				newPositions[i].z += DISTANCE_OFFSET / 2.0f;
				
			}
			else 
			{
				if (i % 2 == 0)
				{
					newPositions[i] = centerPosition;
					newPositions[i].x += (DISTANCE_OFFSET * multiplier2);
				}
				else
				{
					multiplier2++;
					newPositions[i] = centerPosition;
					newPositions[i].x -= (DISTANCE_OFFSET * multiplier2);
				}
				newPositions[i].z -= DISTANCE_OFFSET / 2.0f;
			}
		}
	}

	for (size_t i = 0; i < numMembers; i++)
	{
		glm::vec3 radius = newPositions[i] - centerPosition;
		//float angle = glm::angle(glm::normalize(faceDirection), glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
		float angle = glm::orientedAngle(glm::normalize(faceDirection), glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)), glm::vec3(0.0f, -1.0f, 0.0f));
		newPositions[i] = glm::rotateY(radius, angle) + centerPosition;
	}
}


