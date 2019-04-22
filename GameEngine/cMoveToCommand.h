#ifndef _MOVE_TO_COMMAND_HG_
#define _MOVE_TO_COMMAND_HG_

#include "iCommand.h"

#include <glm/vec3.hpp>

class cMoveToCommand :
	public iCommand
{
public:
	cMoveToCommand();
	virtual ~cMoveToCommand();
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
	float minDistance;
	bool bIsDone;
	bool withCamera;
	glm::vec3 direction;
};

#endif


