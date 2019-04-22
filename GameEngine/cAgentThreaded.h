#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <Windows.h>		// For CRITICAL_SECTION, CreateThread, ResumeThread, etc.
#include <process.h>
#include <string>

#include "CHRTimer.h"
//#include "cBehaviour.h"
//#include "cMeshObject.h"

#include <vector>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

class cAgentBufferManager;
class cRandThreaded;

class cAgentThreaded
{
public:

	enum eAgentBehaviourType
	{
		SEEK,
		FLEE,
		WANDER
	};


	cAgentThreaded();
	~cAgentThreaded();
	// This would create a thread for this Dalek, 
	//	which would call update on this instance
	// The index is so that it can call the triple buffer thing
	void CreateThreadAndStartUpdating(unsigned int dalekIndex,
		cAgentBufferManager* pDalekManager,
		cRandThreaded* pRandThread);

	void CreateThreadAndStartUpdating(unsigned int dalekIndex,
		cRandThreaded* pRandThread);


	// The entire update of whatever it is that the Dalek
	//	is trying to (move, look at things in the world)
	void Update(void);

	// When set to false, the thread will exit, stopping any updates
	bool bIsAlive;

	void ResetTimerAndStart(void);

	// Will pick a random direction and move for x seconds
	void PickNewDirection(void);

	CRITICAL_SECTION csAgent;
	CRITICAL_SECTION csAnim;
	CRITICAL_SECTION csOri;
	//CRITICAL_SECTION csPosRead;

	void readPosition(glm::vec3& pos);
	glm::vec3 readPosition();
	void addDeltaVel(glm::vec3 deltaVel);
	void updatePosition(glm::vec3 pos);
	void setInitialPos(glm::vec3 initialPos);

	glm::quat readOri();
	void updateOri(glm::quat ori);
	//cBehaviour* behaviour;
	//cMeshObject* mTarget;
	void updateTargetPos(glm::vec3 agentPos);
	void updateTargetOri(glm::quat agentOri);

	glm::vec3 targetPos;
	glm::quat targetOri;
	eAgentBehaviourType behaviourType;

	void setBufferManager(cAgentBufferManager* bufferManager);
	void setIndex(unsigned int index);
	void initPosBuffer(glm::vec3 pos);
	void initOriBuffer(glm::quat ori);


	//WANDER
	glm::vec3 currentLocationToGo;
	//!WANDER
private:
	const float AGENT_SPEED = 0.00002f;
	glm::vec3 m_position;
	glm::quat m_ori;
	glm::vec3 m_initPos; 

	unsigned int m_dalekIndex;

	cAgentBufferManager* m_pDalekManager;

	cRandThreaded* m_pRandThread;

	// Object will move for this amount of time
	float m_MoveCountDown;
	glm::vec3 m_Velocity;

	CHRTimer* m_pLocalTimer;
};