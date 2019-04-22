#ifndef _MOVE_TO_COMMAND_EASE_HG_
#define _MOVE_TO_COMMAND_EASE_HG_

#include "iCommand.h"

#include <glm/vec3.hpp>

class cMoveToEaseWithTime :
	public iCommand
{
public:
	cMoveToEaseWithTime();
	virtual ~cMoveToEaseWithTime();
	virtual void initialize(nlohmann::json values);
	virtual void update(double deltaTime);
	virtual bool isFinished(void);
	void setObject(cMeshObject* object);
	cMeshObject* objectToMove;
private:
	double timeElapsed;
	glm::vec3 from;
	glm::vec3 to;
	double time;
	double easeInTime;
	double easeOutTime;
	float minDistance;
	float bIsDone;
	glm::vec3 direction;
	glm::vec3 maxVelocity;
};

#endif