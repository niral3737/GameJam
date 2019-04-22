#ifndef _cLocationTriggerMoveOrient_HG_
#define _cLocationTriggerMoveOrient_HG_

#include "iCommand.h"
#include "cMoveToCommand.h"
#include "cOrientToCommand.h"
class cLocationTriggerMoveOrient :
	public iCommand
{
public:
	cLocationTriggerMoveOrient();
	virtual ~cLocationTriggerMoveOrient();
	virtual void initialize(nlohmann::json values);
	virtual void update(double deltaTime);
	virtual bool isFinished(void);
	void setObject(cMeshObject* object);
	void setTriggeringObject(cMeshObject* object);
	cMeshObject* objectToMove;
	cMeshObject* triggeringObject;
private:
	float triggeringDistance;
	double timeElapsed;
	glm::vec3 fromAngles;
	glm::vec3 toAngles;
	glm::vec3 directionMove;
	glm::vec3 directionAngles;
	glm::vec3 currentAngles;
	double time;
	bool bIsDone;
	glm::vec3 from;
	glm::vec3 to;
	float minDistance;
};

#endif // !_cLocationTriggerMoveOrient_HG_



