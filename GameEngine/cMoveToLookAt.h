#ifndef _MOVE_TO_LOOK_AT_HG_
#define _MOVE_TO_LOOK_AT_HG_

#include "iCommand.h"

#include <glm/vec3.hpp>

class cMoveToLookAt :
	public iCommand
{
public:
	cMoveToLookAt();
	virtual ~cMoveToLookAt();
	virtual void initialize(nlohmann::json values);
	virtual void update(double deltaTime);
	virtual bool isFinished(void);
	void setObject(cMeshObject* object);
	cMeshObject* objectToMove;
private:
	double timeElapsed;
	glm::vec3 from;
	glm::vec3 to;
	glm::vec3 lookAt;
	double time;
	double easeOutTime;
	float minDistance;
	bool bIsDone;
	bool isCamera;
	glm::vec3 direction;
};

#endif // !_MOVE_TO_LOOK_AT_HG_


