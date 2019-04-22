#include "cAgentThreaded.h"

#include <process.h>
#include <Windows.h>		// For CRITICAL_SECTION, CreateThread, ResumeThread, etc.

#include "cRandThreaded.h"
#include "cAgentBufferManager.h"


DWORD WINAPI AgentBrainThread(void* pInitialData)
{
	// Deref the this pointer to get at the specific Dalek

	cAgentThreaded* pBird = (cAgentThreaded*)pInitialData;

	// Delay the start for about 250 ms
	Sleep(250);
	pBird->PickNewDirection();
	// Now endlessly run the loop until it's time to exit
	while (pBird->bIsAlive)
	{
		pBird->Update();
		// Release control of the thread
		Sleep(0);

	}//pDalek->bIsAlive


	return 0;
}

cAgentThreaded::cAgentThreaded()
{
	InitializeCriticalSection(&(this->csAgent));
	InitializeCriticalSection(&(this->csAnim));
	InitializeCriticalSection(&(this->csOri));

	this->bIsAlive = true;
	this->m_dalekIndex = 0;
	this->m_pDalekManager = NULL;

	this->m_pLocalTimer = new CHRTimer();
	this->m_pLocalTimer->Reset();
	return;
}

cAgentThreaded::~cAgentThreaded()
{
	DeleteCriticalSection(&(this->csAgent));
	DeleteCriticalSection(&(this->csAnim));
	DeleteCriticalSection(&(this->csOri));
}

void cAgentThreaded::CreateThreadAndStartUpdating(unsigned int dalekIndex,
	cAgentBufferManager* pDalekManager,
	cRandThreaded* pRandThread)
{
	this->m_dalekIndex = dalekIndex;

	this->m_pDalekManager = pDalekManager;

	this->m_pRandThread = pRandThread;

	this->bIsAlive = true;

	LPDWORD phThread = 0;	// Clear to zero
	DWORD hThread = 0;
	HANDLE hThreadHandle = 0;

	// Pass a pointer to this instance 
	// Recal that the "this" pointer is the pointer to
	//	this particular instance of the object
	void* pThisDalek = (void*)(this);

	hThreadHandle = CreateThread(NULL,	// Default security
		0,		// Stack size - default - win32 = 1 Mbyte
		&AgentBrainThread, // Pointer to the thread's function
		pThisDalek,		// The value (parameter) we pass to the thread
			// This is a pointer to void... more on this evil thing later...
		0,  // CREATE_SUSPENDED or 0 for threads...
		(DWORD*)&phThread);		// pointer or ref to variable that will get loaded with threadID

	return;
}

void cAgentThreaded::ResetTimerAndStart(void)
{
	this->m_pLocalTimer->ResetAndStart();
	return;
}

void cAgentThreaded::PickNewDirection(void)
{
	//behaviour->updateNewPositions();
	//glm::vec3 direction = behaviour->newPositions[m_dalekIndex] - m_initPos;

	//this->m_MoveCountDown = 2000.0f;
	//this->m_Velocity = direction / this->m_MoveCountDown;
}

void cAgentThreaded::Update(void)
{
	glm::vec3 forward = readOri() *  glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 seek = glm::vec3(0.0f);
	glm::vec3 currentPos = readPosition();
	seek = glm::normalize(targetPos - currentPos);
	if (behaviourType == WANDER)
	{
		seek = glm::normalize(currentLocationToGo - currentPos);
	}
	glm::quat orientation = glm::quatLookAt(-seek, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat oriToConsider = glm::quatLookAt(seek, glm::vec3(0.0f, 1.0f, 0.0f));
	bool looking = false;
	if (behaviourType == FLEE)
	{
		float angle = glm::degrees(glm::acos(glm::dot(oriToConsider, targetOri)));
		if ((angle >= 0.0f && angle <= 10.0f) || (angle <= 180.0f && angle >= 170.0f))
		{
			seek = -seek;
			orientation = glm::quatLookAt(-seek, glm::vec3(0.0f, 1.0f, 0.0f));
			looking = true;
		}
	}

	updateOri(orientation);

	float distance = 0.0f;
	distance = glm::distance(targetPos, currentPos);
	if (behaviourType == WANDER)
	{
		distance = glm::distance(currentLocationToGo, currentPos);
	}
	if (distance <= 15.0f)
	{
		if (behaviourType == WANDER)
		{
			float x =  this->m_pRandThread->getNextRandDouble() * 200.0f;
			float z =  this->m_pRandThread->getNextRandDouble() * 200.0f;

			this->currentLocationToGo = glm::vec3(x, 0.0f, z);
		}
		addDeltaVel(glm::vec3(0.0f));
		if (looking)
		{
			glm::vec3 deltaVel = glm::vec3(seek.x * AGENT_SPEED, 0.0f, seek.z * AGENT_SPEED);
			addDeltaVel(deltaVel);
		}
	}
	else
	{
		glm::vec3 deltaVel = glm::vec3(seek.x * AGENT_SPEED, 0.0f, seek.z * AGENT_SPEED);
		addDeltaVel(deltaVel);
	}
}

void cAgentThreaded::readPosition(glm::vec3& pos)
{
	pos = m_pDalekManager->vecPositions[m_pDalekManager->getCurrentReadBuffer()][m_dalekIndex];
}
glm::vec3 cAgentThreaded::readPosition()
{
	//return m_pDalekManager->vecPositions[m_pDalekManager->getCurrentReadBuffer()][m_dalekIndex];
	return m_pDalekManager->readPositionAtIndex(m_dalekIndex);
}

glm::quat cAgentThreaded::readOri()
{
	return m_pDalekManager->readOriAtIndex(m_dalekIndex);
}

void cAgentThreaded::updateOri(glm::quat ori)
{
	this->m_ori = ori;
	m_pDalekManager->UpdateOrientationAtIndex(m_dalekIndex, ori);
}

void cAgentThreaded::updateTargetPos(glm::vec3 agentPos)
{
	EnterCriticalSection(&(this->csAgent));
	this->targetPos = agentPos;
	LeaveCriticalSection(&(this->csAgent));
}

void cAgentThreaded::updateTargetOri(glm::quat agentOri)
{
	EnterCriticalSection(&(this->csAgent));
	this->targetOri = agentOri;
	LeaveCriticalSection(&(this->csAgent));
}

void cAgentThreaded::addDeltaVel(glm::vec3 deltaVel)
{
	this->m_position += deltaVel;
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, this->m_position);
}

void cAgentThreaded::updatePosition(glm::vec3 pos)
{
	this->m_position = pos;
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, pos);
}

void cAgentThreaded::setInitialPos(glm::vec3 initialPos)
{
	this->m_position = initialPos;
	this->m_initPos = initialPos;
}

void cAgentThreaded::setBufferManager(cAgentBufferManager * bufferManager)
{
	this->m_pDalekManager = bufferManager;
}

void cAgentThreaded::setIndex(unsigned int index)
{
	this->m_dalekIndex = index;
}

void cAgentThreaded::initPosBuffer(glm::vec3 pos)
{
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, pos);
	m_pDalekManager->SwitchBuffers();
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, pos);
	m_pDalekManager->SwitchBuffers();
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, pos);
	m_pDalekManager->SwitchBuffers();
}

void cAgentThreaded::initOriBuffer(glm::quat ori)
{
	m_pDalekManager->UpdateOrientationAtIndex(m_dalekIndex, ori);
	m_pDalekManager->SwitchBuffers();
	m_pDalekManager->UpdateOrientationAtIndex(m_dalekIndex, ori);
	m_pDalekManager->SwitchBuffers();
	m_pDalekManager->UpdateOrientationAtIndex(m_dalekIndex, ori);
	m_pDalekManager->SwitchBuffers();
}