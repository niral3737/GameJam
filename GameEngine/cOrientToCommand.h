#ifndef _ORIENT_TO_COMMAND_HG_
#define _ORIENT_TO_COMMAND_HG_

#include "iCommand.h"

class cOrientToCommand :
	public iCommand
{
public:
	cOrientToCommand();
	virtual ~cOrientToCommand();
	virtual void initialize(nlohmann::json values);
	virtual void update(double deltaTime);
	virtual bool isFinished(void);
	void setObject(cMeshObject* object);
	cMeshObject* object;
private:
	double timeElapsed;
	glm::vec3 fromAngles;
	glm::vec3 toAngles;
	glm::vec3 direction;
	glm::vec3 currentAngles;
	double time;
	bool bIsDone;
};

#endif