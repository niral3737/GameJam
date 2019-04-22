#include "cAnimationCurveCommand.h"
#include <fstream>
#include <iostream>

cAnimationCurveCommand::cAnimationCurveCommand()
{
	this->object = NULL;
}


cAnimationCurveCommand::~cAnimationCurveCommand()
{}

void cAnimationCurveCommand::initialize(nlohmann::json values)
{
	std::string positionWayPointsFileName = values["PositionWayPointsFileName"].get<std::string>();
	std::ifstream posFile(positionWayPointsFileName.c_str());
	
	std::string nextData;
	while (posFile >> nextData)
	{
		if (nextData == "points")
		{
			break;		// exit while loop...
		}
	};
	unsigned int numPosPoints;
	posFile >> numPosPoints;
	for (size_t i = 0; i < numPosPoints; i++)
	{
		glm::vec3 wayPoint;
		posFile >> wayPoint.x;
		posFile >> wayPoint.y;
		posFile >> wayPoint.z;
		this->m_vecPositionSteps.push_back(wayPoint);
	}
	posFile.close();



	std::string orientationWayPointsFileName = values["OrientationWayPointsFileName"].get<std::string>();
	std::ifstream oFile(orientationWayPointsFileName.c_str());

	nextData = "";
	while (oFile >> nextData)
	{
		if (nextData == "points")
		{
			break;		// exit while loop...
		}
	};

	oFile >> numPosPoints;
	for (size_t i = 0; i < numPosPoints; i++)
	{
		glm::vec3 wayPoint;
		oFile >> wayPoint.x;
		oFile >> wayPoint.y;
		oFile >> wayPoint.z;
		this->m_vecOrientationSteps.push_back(wayPoint);
	}
	oFile.close();

	this->m_frameRate = 0.5f;		// 32 ms

	this->m_frameTime = 1.0f / this->m_frameRate;	// around 32 ms

	this->m_frameNumber = 0;			// Start at frame 0
	this->m_currentFrameTime = 0.0f;	// Start of animation (time = 0)

	// 
	this->currentLocation = this->m_vecPositionSteps[0];

	return;
}

void cAnimationCurveCommand::update(double deltaTime)
{
	if (this->m_bIsDone)
	{
		return;
	}

	// 0.001 .002 0.003   0.033?
	this->m_currentFrameTime += (float)deltaTime;

	if (this->m_currentFrameTime >= this->m_frameTime)
	{
		this->m_frameNumber++;

		// We could reset to zero, but there will alway be a "little bit of time" left
		//this->m_currentFrameTime = 0.0f;

		// Instead, we take away the frame time (33ms)
		// Leaving the "left over bit"
		this->m_currentFrameTime -= this->m_frameTime;
	}

	// Have I played the last frame? 
	if (this->m_frameNumber >= this->m_vecPositionSteps.size())
	{
		// Yes
		this->m_bIsDone = true;
		return;
	}

	this->m_Update_Position(deltaTime);
	this->m_Update_Orientation(deltaTime);

	return;
}

bool cAnimationCurveCommand::isFinished(void)
{
	return this->m_bIsDone;
}

void cAnimationCurveCommand::setObject(cMeshObject * object)
{
	this->object = object;
}

// I pass a number between 0 and 1 and return a scaled value
// between the min and max...
float cAnimationCurveCommand::m_LERP(float minValue, float maxValue, float valueInBetween)
{
	// Or "interpolated" value
	float LEFRP_Value = ((maxValue - minValue) * valueInBetween) + minValue;

	return LEFRP_Value;
}

void cAnimationCurveCommand::m_Update_Position(double deltaTime)
{
	// At this point we have a valid frame.... 
// Look up the position (or whatever) information
//this->currentLocation = this->m_vecPositionSteps[this->m_frameNumber];

// Use LERP to see how far in between these values we are:
// What value was the LAST frame? 
	glm::vec3 thisFramePos = this->m_vecPositionSteps[this->m_frameNumber];

	// What value is the NEXT frame? 
	unsigned int nextFrameIndex = this->m_frameNumber + 1;
	// Are we off the end of our vector of animation frames? 
	if (nextFrameIndex >= this->m_vecPositionSteps.size())
	{
		nextFrameIndex = this->m_vecOrientationSteps.size() - 1;
	}
	glm::vec3 nextFramePos = this->m_vecPositionSteps[nextFrameIndex];


	// What's the "in between value" we WOULD have (if we had it)? 
	// Returns a value between 0 and 1
	float ratioIntoFrame = this->m_currentFrameTime / this->m_frameTime;
	if (ratioIntoFrame > 1.0f)
	{
		ratioIntoFrame = 1.0f;
	}

	this->currentLocation.x = this->m_LERP(thisFramePos.x, nextFramePos.x, this->m_currentFrameTime);
	this->currentLocation.y = this->m_LERP(thisFramePos.y, nextFramePos.y, this->m_currentFrameTime);
	this->currentLocation.z = this->m_LERP(thisFramePos.z, nextFramePos.z, this->m_currentFrameTime);

	this->object->position = currentLocation;
}

void cAnimationCurveCommand::m_Update_Orientation(double deltaTime)
{
	// Would do a SLERP here
	this->qCurrentOrientation = glm::quat(this->m_vecOrientationSteps[this->m_frameNumber]);
	this->object->setOrientation(this->qCurrentOrientation);
	return;
}