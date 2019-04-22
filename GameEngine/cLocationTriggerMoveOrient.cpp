#include "cLocationTriggerMoveOrient.h"

#include <iostream>

cLocationTriggerMoveOrient::cLocationTriggerMoveOrient()
{}


cLocationTriggerMoveOrient::~cLocationTriggerMoveOrient()
{}

void cLocationTriggerMoveOrient::initialize(nlohmann::json values)
{
	this->from = this->objectToMove->position;
	glm::vec3 eularAngles = glm::eulerAngles(objectToMove->getOrientation());

	this->fromAngles = glm::degrees(eularAngles);
	objectToMove->setOrientationEulerAngles(this->fromAngles, true);
	this->currentAngles = this->fromAngles;
	this->triggeringDistance = values["triggerDistance"].get<float>();

	this->to.x = values["to"]["x"].get<float>();
	this->to.y = values["to"]["y"].get<float>();
	this->to.z = values["to"]["z"].get<float>();

	this->toAngles.x = values["toAngles"]["x"].get<float>();
	this->toAngles.y = values["toAngles"]["y"].get<float>();
	this->toAngles.z = values["toAngles"]["z"].get<float>();

	this->time = values["time"].get<double>();

	this->minDistance = 1.0f;
	this->bIsDone = false;
	this->timeElapsed = 0.0;
	this->directionMove = this->to - this->from;
	this->directionAngles = this->toAngles - this->fromAngles;
}

void cLocationTriggerMoveOrient::update(double deltaTime)
{
	float distanceBWTwoObjects = glm::distance(this->triggeringObject->position, this->objectToMove->position);

	if (distanceBWTwoObjects > this->triggeringDistance)
	{
		return;
	}
	double offset = deltaTime / this->time;
	glm::vec3 stepInDirectionMove = glm::vec3(this->directionMove.x * offset,
		this->directionMove.y * offset,
		this->directionMove.z * offset);

	glm::vec3 stepInDirectionAngles = glm::vec3(this->directionAngles.x * offset,
		this->directionAngles.y * offset,
		this->directionAngles.z * offset);

	this->currentAngles += stepInDirectionAngles;
	this->objectToMove->setOrientationEulerAngles(currentAngles, true);
	this->objectToMove->position += stepInDirectionMove;

	this->timeElapsed += deltaTime;
	float currentDistance = glm::distance(this->to, this->from);

	if (timeElapsed >= this->time)
	{
		// Yes, like Lady Gaga, I have arrived...
		this->bIsDone = true;
		this->objectToMove->position = this->to;
		this->objectToMove->setOrientationEulerAngles(toAngles, true);
		return;
	}
}

bool cLocationTriggerMoveOrient::isFinished(void)
{
	return this->bIsDone;
}

void cLocationTriggerMoveOrient::setObject(cMeshObject * object)
{
	this->objectToMove = object;
}

void cLocationTriggerMoveOrient::setTriggeringObject(cMeshObject * object)
{
	this->triggeringObject = object;
}