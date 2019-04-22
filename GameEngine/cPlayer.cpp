#include "cPlayer.h"


cPlayer::cPlayer()
{
	this->mesh = NULL;
	this->at = glm::vec3(0.0f, 0.0f, 1.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
}


cPlayer::~cPlayer()
{}


void cPlayer::move(float amount)
{
	glm::mat4 matRotation = glm::mat4(mesh->getOrientation());
	glm::vec4 frontOfShip = glm::vec4(front, 1.0f);
	at = matRotation * frontOfShip;

	glm::vec3 direction = glm::vec3(at);
	direction = glm::normalize(direction);
	glm::vec3 amountToMove = direction * amount;
	this->previousPos = mesh->position;
	mesh->position += amountToMove;
}

void cPlayer::shootCannonBall()
{
	//TODO handle multiple ball shooting
	updateAt();
	cCannonBall* ball = cannonBalls[0];
	ball->mesh->position = this->mesh->position;
	ball->direction = this->at;
	ball->isShot = true;
}

void cPlayer::update(float dt)
{
	mDt = dt;
	for (size_t i = 0; i < cannonBalls.size(); i++)
	{
		cannonBalls[i]->previousPos = cannonBalls[i]->mesh->position;
		if (cannonBalls[i]->isShot)
		{
			cannonBalls[i]->mesh->position += (cannonBalls[i]->direction) * 2.0f;
			if (cannonBalls[i]->mesh->position.x < -120.0f ||
				cannonBalls[i]->mesh->position.x > 600.0f ||
				cannonBalls[i]->mesh->position.z < -660.0f ||
				cannonBalls[i]->mesh->position.z > -330.0f)
			{
				cannonBalls[i]->mesh->position = mesh->position;
				cannonBalls[i]->isShot = false;
			}
		}
		else
		{
			cannonBalls[i]->mesh->position = mesh->position;
		}
	}
}

std::string cPlayer::getName()
{
	if (this->mesh == NULL)
	{
		return "";
	}
	return this->mesh->friendlyName;
}

cMeshObject * cPlayer::GetMesh()
{
	return this->mesh;
}

void cPlayer::updateAt()
{
	glm::mat4 matRotation = glm::mat4(mesh->getOrientation());
	glm::vec4 frontOfShip = glm::vec4(front, 1.0f);
	at = matRotation * frontOfShip;
}

glm::vec3 cPlayer::getPreviousPos()
{
	return this->previousPos;
}


