#include "cOrientToCommand.h"



cOrientToCommand::cOrientToCommand()
{
	this->object = NULL;
}


cOrientToCommand::~cOrientToCommand()
{}

void cOrientToCommand::initialize(nlohmann::json values)
{
	this->fromAngles.x = values["fromAngles"]["x"].get<float>();
	this->fromAngles.y = values["fromAngles"]["y"].get<float>();
	this->fromAngles.z = values["fromAngles"]["z"].get<float>();
	this->object->setOrientationEulerAngles(fromAngles, true);
	this->currentAngles = fromAngles;

	this->toAngles.x = values["toAngles"]["x"].get<float>();
	this->toAngles.y = values["toAngles"]["y"].get<float>();
	this->toAngles.z = values["toAngles"]["z"].get<float>();

	this->time = values["time"].get<double>();
	this->bIsDone = false;
	this->timeElapsed = 0.0;
	this->direction = this->toAngles - this->fromAngles;
}

void cOrientToCommand::update(double deltaTime)
{
	double offset = deltaTime / this->time;
	glm::vec3 stepInDirection = glm::vec3(this->direction.x * offset,
		this->direction.y * offset,
		this->direction.z * offset);
	this->currentAngles += stepInDirection;
	this->object->setOrientationEulerAngles(currentAngles, true);
	this->timeElapsed += deltaTime;

	if (timeElapsed >= this->time)
	{
		this->bIsDone = true;
		this->object->setOrientationEulerAngles(toAngles, true);
		return;
	}
}

bool cOrientToCommand::isFinished(void)
{
	return this->bIsDone;
}

void cOrientToCommand::setObject(cMeshObject * object)
{
	this->object = object;
}