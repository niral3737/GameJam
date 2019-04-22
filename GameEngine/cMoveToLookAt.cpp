#include "cMoveToLookAt.h"
#include "cCamera.h"


cMoveToLookAt::cMoveToLookAt()
{
	this->objectToMove = NULL;
}


cMoveToLookAt::~cMoveToLookAt()
{}

void cMoveToLookAt::initialize(nlohmann::json values)
{
	this->from.x = values["from"]["x"].get<float>();
	this->from.y = values["from"]["y"].get<float>();
	this->from.z = values["from"]["z"].get<float>();

	this->to.x = values["to"]["x"].get<float>();
	this->to.y = values["to"]["y"].get<float>();
	this->to.z = values["to"]["z"].get<float>();

	this->lookAt.x = values["lookAt"]["x"].get<float>();
	this->lookAt.y = values["lookAt"]["y"].get<float>();
	this->lookAt.z = values["lookAt"]["z"].get<float>();

	this->time = values["time"].get<double>();
	this->easeOutTime = values["easeOutTime"].get<double>();

	this->isCamera = values["isCamera"].get<bool>();

	if (isCamera)
	{
		//cCamera::getInstance()->eye = this->from;
	}
	else
	{
		this->objectToMove->position = this->from;
	}

	this->minDistance = 1.0f;
	this->bIsDone = false;
	this->timeElapsed = 0.0;
	this->direction = this->to - this->from;
}

void cMoveToLookAt::update(double deltaTime)
{
	double offset = deltaTime / this->time;
	glm::vec3 stepInDirection = glm::vec3(this->direction.x * offset,
		this->direction.y * offset,
		this->direction.z * offset);

	if (this->isCamera)
	{
		cCamera::getInstance()->eye += stepInDirection;
		cCamera::getInstance()->lookAt(this->lookAt);
	}
	else
	{
		this->objectToMove->position += stepInDirection;
	}
	this->timeElapsed += deltaTime;
	float currentDistance = glm::distance(this->to, this->from);

	if (timeElapsed >= this->time)
	{
		this->bIsDone = true;
		if (this->isCamera)
		{
			cCamera::getInstance()->eye += stepInDirection;
			cCamera::getInstance()->lookAt(this->lookAt);
		}
		else
		{
			this->objectToMove->position = this->to;
		}
		return;
	}
}

bool cMoveToLookAt::isFinished(void)
{
	return this->bIsDone;
}

void cMoveToLookAt::setObject(cMeshObject * object)
{
	this->objectToMove = object;
}
