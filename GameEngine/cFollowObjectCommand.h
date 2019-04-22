#ifndef _cFollowObjectCommand_HG_
#define _cFollowObjectCommand_HG_

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cMeshObject.h"

class cFollowObjectCommand : public iCommand
{
public:
	cFollowObjectCommand();
	~cFollowObjectCommand();

	virtual void initialize(nlohmann::json values);

	// These are both called every frame 
	virtual void update(double deltaTime);
	virtual bool isFinished(void);

	glm::vec3 currentLocation;
	cMeshObject* object;
	void setObject(cMeshObject* object);
private:
	glm::vec3 idealCameraLocation;

	float minDistance;	// Any less than this and camera stops

	float maxSpeedDistance;	// If MORE than this far away...
	// ... go at this maximum speed
	float maxSpeed;

	bool m_bIsDone;
	bool isCamera;

	glm::vec3 m_adjustVelocity(glm::vec3 vVel);
	glm::vec3 m_adjustVelocity_2(glm::vec3 vVel);
};

#endif