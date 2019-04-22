#include "cCharacter.h"
#include <process.h>
#include <Windows.h>

cCharacter::cCharacter()
{
	this->mesh = NULL;
	this->at = glm::vec3(0.0f, 0.0f, 1.0f);
	this->isRunning = false;
	this->isJumping = false;
	this->isColliding = false;
}

cCharacter::~cCharacter()
{
}


void cCharacter::move(float amount)
{
	if (isColliding == true)
	{
		isColliding == false;
		if (amount < 0.0f)
		{
			this->mesh->currentAnimation = "Walk-Back";
			goInDir(amount);
			isColliding == false;
		}
		return;
	}
	if (isJumping)
	{
		goInDir(amount + 0.20f);
		return;
	}
	this->mesh->currentAnimation = "Unarmed-Walk";
	if (isRunning)
	{
		this->mesh->currentAnimation = "Run";
	}
	if (amount < 0.0f)
	{
		this->mesh->currentAnimation = "Walk-Back";
	}
	isColliding == false;
	goInDir(amount);
}
void cCharacter::goInDir(float amount)
{
	glm::mat4 matRotation = glm::mat4(mesh->getOrientation());
	glm::vec4 frontOfCharacter = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	this->at = glm::normalize(matRotation * frontOfCharacter);
	mesh->position += this->at * amount;

	//addToPosition(this->at * amount);
	/*glm::vec3 position = mesh->rigidBody->GetPosition();
	position += this->at * amount;
	mesh->rigidBody->SetPosition(position);*/

	/*glm::vec3 vel = mesh->rigidBody->GetVelocity();
	vel += this->at * amount;
	mesh->rigidBody->SetVelocity(vel);*/
}

void cCharacter::strafe(float amount)
{
	this->mesh->currentAnimation = "Strafe-Right";
	if (amount < 0.0f)
	{
		this->mesh->currentAnimation = "Strafe-Left";
	}

	glm::mat4 matRotation = glm::mat4(mesh->getOrientation());
	glm::vec4 rightOfCharacter = glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::normalize(matRotation * rightOfCharacter);
	mesh->position += right * amount;
	//addToPosition(right * amount);
	//glm::vec3 position = mesh->rigidBody->GetPosition();
	//position += right * amount;
	//mesh->rigidBody->SetPosition(position);
}


void cCharacter::jump()
{
	this->mesh->currentAnimation = "Jump";
	isJumping = true;
}

void cCharacter::action()
{
	this->mesh->currentAnimation = "Action";
}

