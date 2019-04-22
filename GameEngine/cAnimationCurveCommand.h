#ifndef _cAnimationCurveCommand_HG_
#define _cAnimationCurveCommand_HG_

#include "iCommand.h"
class cAnimationCurveCommand :
	public iCommand
{
public:
	cAnimationCurveCommand();
	virtual ~cAnimationCurveCommand();
	virtual void initialize(nlohmann::json values);
	virtual void update(double deltaTime);
	virtual bool isFinished(void);
	void setObject(cMeshObject* object);
	cMeshObject* object;

	// I am reading this every frame
	glm::vec3 currentLocation;

	glm::quat qCurrentOrientation;
private:
	void m_Update_Position(double deltaTime);
	void m_Update_Orientation(double deltaTime);

	// I pass a number between 0 and 1 and return a scaled value
	// between the min and max...
	float m_LERP(float minValue, float maxValue, float amountDone_0_to_1);

	bool m_bIsDone;

	float m_frameRate;	// Frames per second (FPS)

	float m_frameTime;	// Seconds between animation steps

	// How far into THIS frame am I?
	float m_currentFrameTime;


	unsigned int m_frameNumber;	// Start at 0 

	// Animation steps we will play back
	std::vector< glm::vec3 > m_vecPositionSteps;
	std::vector< glm::vec3 > m_vecOrientationSteps;
	// You could have any number of sets of these things
	//std::vector< glm:vec3 > m_vecOrientationSteps;
	//std::vector< glm:vec3 > m_vecColourSteps;
};


#endif