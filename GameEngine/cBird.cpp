#include "cBird.h"

#include <process.h>
#include <Windows.h>		// For CRITICAL_SECTION, CreateThread, ResumeThread, etc.

#include <iostream>
#include "cRandThreaded.h"
#include "cBirdBufferManager.h"

DWORD WINAPI BirdBrainThread(void* pInitialData)
{
	cBird* pBird = (cBird*)pInitialData;

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

cBird::cBird()
{
	InitializeCriticalSection(&(this->csPosition));

	this->bIsAlive = true;
	this->m_dalekIndex = 0;
	this->m_pDalekManager = NULL;

	this->m_pLocalTimer = new CHRTimer();
	this->m_pLocalTimer->Reset();
	this->behaviour = NULL;
	return;
}

cBird::~cBird()
{
	DeleteCriticalSection(&(this->csPosition));
}

void cBird::CreateThreadAndStartUpdating(unsigned int dalekIndex,
	cBirdBufferManager* pDalekManager,
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
		&BirdBrainThread, // Pointer to the thread's function
		pThisDalek,		// The value (parameter) we pass to the thread
			// This is a pointer to void... more on this evil thing later...
		0,  // CREATE_SUSPENDED or 0 for threads...
		(DWORD*)&phThread);		// pointer or ref to variable that will get loaded with threadID

	return;
}

void cBird::ResetTimerAndStart(void)
{
	this->m_pLocalTimer->ResetAndStart();
	return;
}

void cBird::PickNewDirection(void)
{
	//behaviour->updateNewPositions();
	glm::vec3 direction = behaviour->newPositions[m_dalekIndex] - m_initPos;
	
	this->m_MoveCountDown = 2000.0f;
	this->m_Velocity = direction / this->m_MoveCountDown;
}

void cBird::Update(void)
{
	//float deltaTime = this->m_pLocalTimer->GetElapsedSecondsAverage();
	//float deltaTime = 0.00001f;
	float deltaTime = 0.01f;
	glm::vec3 deltaVel = this->m_Velocity * deltaTime;

	if (this->m_MoveCountDown <= 0.0f)
	{
		updatePosition(behaviour->newPositions[m_dalekIndex]);
		this->m_pDalekManager->SwitchBuffers();
		updatePosition(behaviour->newPositions[m_dalekIndex]);
		this->m_pDalekManager->SwitchBuffers();
		updatePosition(behaviour->newPositions[m_dalekIndex]);
		//this->m_pDalekManager->SwitchBuffers();
		this->bIsAlive = false;
	}
	else
	{
		addDeltaVel(deltaVel);

		this->m_MoveCountDown -= deltaTime;
	}
	return;
}

void cBird::readPosition(glm::vec3& pos)
{
	/*EnterCriticalSection(&(this->csPosition));
	pos = this->m_position;
	LeaveCriticalSection(&(this->csPosition));*/
	pos = m_pDalekManager->vecPositions[m_pDalekManager->getCurrentReadBuffer()][m_dalekIndex];
}
glm::vec3 cBird::readPosition()
{
	//EnterCriticalSection(&(this->csPosition));
	//glm::vec3 pos = this->m_position;
	//LeaveCriticalSection(&(this->csPosition));
	//return pos;

	return m_pDalekManager->vecPositions[m_pDalekManager->getCurrentReadBuffer()][m_dalekIndex];
}

void cBird::addDeltaVel(glm::vec3 deltaVel)
{
	//EnterCriticalSection(&(this->csPosition));
	this->m_position += deltaVel;
	//LeaveCriticalSection(&(this->csPosition));

	//this->m_position += deltaVel;
	//glm::vec3 pos = readPosition();
	//pos += deltaVel;
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, this->m_position);
	//m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, pos);
}

void cBird::updatePosition(glm::vec3 pos)
{
	//EnterCriticalSection(&(this->csPosition));
	//this->m_position = pos;
	//LeaveCriticalSection(&(this->csPosition));
	this->m_position = pos;
	m_pDalekManager->UpdatePositionAtIndex(m_dalekIndex, pos);
}

void cBird::setInitialPos(glm::vec3 initialPos)
{
	this->m_position = initialPos;
	this->m_initPos = initialPos;
}
