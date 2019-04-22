#include "cMoveToEaseWithTime.h"



cMoveToEaseWithTime::cMoveToEaseWithTime()
{
	this->objectToMove = NULL;
}


cMoveToEaseWithTime::~cMoveToEaseWithTime()
{}

void cMoveToEaseWithTime::initialize(nlohmann::json values)
{
	this->from.x = values["from"]["x"].get<float>();
	this->from.y = values["from"]["y"].get<float>();
	this->from.z = values["from"]["z"].get<float>();
	this->objectToMove->position = this->from;

	this->to.x = values["to"]["x"].get<float>();
	this->to.y = values["to"]["y"].get<float>();
	this->to.z = values["to"]["z"].get<float>();

	this->time = values["time"].get<double>();
	this->easeInTime = values["easeInTime"].get<double>();
	this->easeOutTime = values["easeOutTime"].get<double>();

	this->minDistance = 1.0f;
	this->bIsDone = false;
	this->timeElapsed = 0.0;
	this->direction = this->to - this->from;
	this->maxVelocity = this->direction / (float)this->time;
}

void cMoveToEaseWithTime::update(double deltaTime)
{
	if (this->timeElapsed <= this->easeInTime)
	{
		float currentDistance = glm::distance(this->to, this->from);

		float ratio = smoothstep_wikipedia(0.0f, 1.0f, this->timeElapsed / this->easeInTime);

		glm::vec3 velocity = maxVelocity * ratio;
		this->objectToMove->position = velocity * (float)deltaTime;
	}else
	{
		double offset = deltaTime / this->time;
		glm::vec3 stepInDirection = glm::vec3(this->direction.x * offset,
			this->direction.y * offset,
			this->direction.z * offset);
		this->objectToMove->position += stepInDirection;
	}
	this->timeElapsed += deltaTime;
	

	if (timeElapsed >= this->time)
	{
		// Yes, like Lady Gaga, I have arrived...
		this->bIsDone = true;
		this->objectToMove->position = this->to;
		return;
	}
}

bool cMoveToEaseWithTime::isFinished(void)
{
	return this->bIsDone;
}

void cMoveToEaseWithTime::setObject(cMeshObject * object)
{
	this->objectToMove = object;
}