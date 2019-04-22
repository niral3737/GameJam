#include "cFollowObjectCommand.h"

#include "cCamera.h"

cFollowObjectCommand::cFollowObjectCommand()
{
	this->m_bIsDone = false;
	this->object = NULL;
}


cFollowObjectCommand::~cFollowObjectCommand()
{}

void cFollowObjectCommand::initialize(nlohmann::json values)
{
	this->idealCameraLocation.x = values["idealLocation"]["x"].get<float>();
	this->idealCameraLocation.y = values["idealLocation"]["y"].get<float>();
	this->idealCameraLocation.z = values["idealLocation"]["z"].get<float>();

	this->minDistance = values["minDistance"].get<float>();
	this->maxSpeedDistance = values["maxSpeedDistance"].get<float>();
	this->maxSpeed = values["maxSpeed"].get<float>();

	this->isCamera = values["isCamera"].get<bool>();

	this->currentLocation.x = values["currentLocation"]["x"].get<float>();
	this->currentLocation.y = values["currentLocation"]["y"].get<float>();
	this->currentLocation.z = values["currentLocation"]["z"].get<float>();
}

void cFollowObjectCommand::update(double deltaTime)
{
	// Get the information from the object that is moving
	float currentDistance = glm::distance(this->idealCameraLocation, this->currentLocation);

	if (currentDistance <= this->minDistance)
	{
		// Yes, like Lady Gaga, I have arrived...
		this->m_bIsDone = true;
		return;
	}

	// Here, I am NOT where I'm supposed to be, so I need to move.
	// Calculate the direction vector to the target location
	glm::vec3 vDirection = this->idealCameraLocation - this->currentLocation;

	// To calc velocity, I will normalize
	glm::vec3 vVel = glm::normalize(vDirection);


	// Adjust velocity in the "slow down" zone
//	vVel = this->m_adjustVelocity( vVel );
	vVel = this->m_adjustVelocity_2(vVel);


	// How far to move in THIS time step?
	glm::vec3 deltaMove = vVel * (float)deltaTime;

	this->currentLocation += deltaMove;
	if (!isCamera)
	{
		object->position = this->currentLocation;
	}
	else
	{
		cCamera::getInstance()->eye = this->currentLocation;
	}
	return;
}

bool cFollowObjectCommand::isFinished(void)
{
	return this->m_bIsDone;
}

glm::vec3 cFollowObjectCommand::m_adjustVelocity(glm::vec3 vVel)
{
	// Scale this with the speed we want to go...

	float currentDistance = glm::distance(this->idealCameraLocation, this->currentLocation);

	// At this point the MAX speed (and direction) is calculated.
	// Smooth step part....

	// Are we outside the "maximum speed zone"?
	if (currentDistance >= this->maxSpeedDistance)
	{
		// Stay at max speed (redundant...)
		vVel = vVel * this->maxSpeed;
		//		std::cout << "Going at max speed" << std::endl;
	}
	else if (currentDistance <= this->minDistance)
	{
		// Stop the camera 
		vVel = glm::vec3(0.0f, 0.0f, 0.0f);
		//		std::cout << "Going at min speed" << std::endl;
	}
	else
	{
		// I'm in the "in between spot" (I'm "slowing down...)

		// Smoothstep (and most of these function) need a value from 0 to 1
		float lengthOfSlowDownArea = this->maxSpeedDistance - this->minDistance;
		// 50.0 long

		float howFarFromMinDistance = currentDistance - this->minDistance;
		// 0.0 away from the "black circle" (speed = 0)

		float HowFarIntoSlowDownZone_0_to_1 = howFarFromMinDistance / lengthOfSlowDownArea;

		float HowFarIntoSlowDownZone_1_to_0 = 1.0f - HowFarIntoSlowDownZone_0_to_1;

		// Ramp function:
//		float slowDownRatio = HowFarIntoSlowDownZone_0_to_1;

//		// Smooth step function:
		float slowDownRatio = smoothstep_wikipedia(1.0f, 0.0f, HowFarIntoSlowDownZone_1_to_0);

		// glm::smoothstep takes a vec3....
 //		float slowDownRatio = glm::smoothstep( this->maxSpeed,  minSpeed, howFarIntoSlowDownZone_0_to_1 );

//		std::cout << "slowDownRatio 1: " << slowDownRatio << std::endl;

		// slowDownRatio is from 0.0 to 1.0
		vVel = vVel * (maxSpeed * slowDownRatio);
	}

	return vVel;
}

glm::vec3 cFollowObjectCommand::m_adjustVelocity_2(glm::vec3 vVel)
{
	float currentDistance = glm::distance(this->idealCameraLocation, this->currentLocation);

	float distance = clamp(this->minDistance, this->maxSpeedDistance, currentDistance);

	// Now distance will be MAX this->maxSpeedDistance...
	// Scale this from 0 to 1 (1 at max distance)
	float slowDownRatio = distance / (this->maxSpeedDistance - this->minDistance);

	// Smoothstep
	slowDownRatio = smoothstep_wikipedia(0.0f, 1.0f, slowDownRatio);

	//	std::cout << "slowDownRatio = " << slowDownRatio << std::endl;

	vVel *= (slowDownRatio * this->maxSpeed);

	return vVel;
}


void cFollowObjectCommand::setObject(cMeshObject * object)
{
	this->object = object;
}