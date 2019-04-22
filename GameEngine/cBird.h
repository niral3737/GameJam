#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <Windows.h>		// For CRITICAL_SECTION, CreateThread, ResumeThread, etc.
#include <process.h>

#include "CHRTimer.h"
#include "FormationBehaviour.h"

class cBirdBufferManager;
class cRandThreaded;

class cBird
{
public:
	cBird();
	~cBird();
	// This would create a thread for this Dalek, 
	//	which would call update on this instance
	// The index is so that it can call the triple buffer thing
	void CreateThreadAndStartUpdating(unsigned int dalekIndex,
		cBirdBufferManager* pDalekManager,
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

	CRITICAL_SECTION csPosition;
	//CRITICAL_SECTION csPosRead;

	void readPosition(glm::vec3& pos);
	glm::vec3 readPosition();
	void addDeltaVel(glm::vec3 deltaVel);
	void updatePosition(glm::vec3 pos);
	void setInitialPos(glm::vec3 initialPos);
	const float CUBE_LENGTH = 20.0f;
	const unsigned int MAZE_HEIGHT = 100;
	const unsigned int MAZE_WIDTH = 100;
	FormationBehaviour* behaviour;
private:
	glm::vec3 m_position;
	glm::vec3 m_initPos;

	unsigned int m_dalekIndex;

	cBirdBufferManager* m_pDalekManager;

	cRandThreaded* m_pRandThread;

	// Object will move for this amount of time
	float m_MoveCountDown;
	glm::vec3 m_Velocity;

	CHRTimer* m_pLocalTimer;
};

