#include "cAgentBufferManager.h"
#include <process.h>
#include <Windows.h>
#include <iostream>

CRITICAL_SECTION CSBufferIndexAgent;
CRITICAL_SECTION CSPos;

cAgentBufferManager::cAgentBufferManager()
{
	InitializeCriticalSection(&CSBufferIndexAgent);
	InitializeCriticalSection(&CSPos);

	this->m_CurrentWriteBufferIndex = 0;
	this->m_CurrentReadBufferIndex = 1;

	return;
}

cAgentBufferManager::~cAgentBufferManager()
{
	DeleteCriticalSection(&CSBufferIndexAgent);
	DeleteCriticalSection(&CSPos);

	return;
}

void cAgentBufferManager::InitBuffers(unsigned int size)
{
	// Create three buffers that are size big...
	// (and fill them will all zeros)
	for (unsigned int buffIndex = 0; buffIndex != NUMBEROFBUFFERS; buffIndex++)
	{
		this->vecPositions[buffIndex].reserve(size);

		for (unsigned int posCount = 0; posCount != size; posCount++)
		{
			this->vecPositions[buffIndex].push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	return;
}

void cAgentBufferManager::InitPosBuffer(unsigned int size, glm::vec3 initPos)
{
	for (unsigned int buffIndex = 0; buffIndex != NUMBEROFBUFFERS; buffIndex++)
	{
		this->vecPositions[buffIndex].reserve(size);

		for (unsigned int posCount = 0; posCount != size; posCount++)
		{
			this->vecPositions[buffIndex].push_back(initPos);
		}
	}

	return;
}
void cAgentBufferManager::InitOriBuffer(unsigned int size, glm::quat initOrientation)
{
	for (unsigned int buffIndex = 0; buffIndex != NUMBEROFBUFFERS; buffIndex++)
	{
		this->vecOrientations[buffIndex].reserve(size);

		for (unsigned int posCount = 0; posCount != size; posCount++)
		{
			this->vecOrientations[buffIndex].push_back(initOrientation);
		}
	}

	return;
}

void cAgentBufferManager::SwitchBuffers(void)
{
	EnterCriticalSection(&CSBufferIndexAgent);

	this->m_CurrentWriteBufferIndex++;
	this->m_CurrentReadBufferIndex++;

	if (this->m_CurrentReadBufferIndex >= NUMBEROFBUFFERS)
	{
		this->m_CurrentReadBufferIndex = 0;
	}
	if (this->m_CurrentWriteBufferIndex >= NUMBEROFBUFFERS)
	{
		this->m_CurrentWriteBufferIndex = 0;
	}

	LeaveCriticalSection(&CSBufferIndexAgent);
	return;
}

unsigned int cAgentBufferManager::getCurrentReadBuffer(void)
{
	return this->m_CurrentReadBufferIndex;
}

unsigned int cAgentBufferManager::getCurrentWriteBuffer(void)
{
	return this->m_CurrentWriteBufferIndex;
}

void cAgentBufferManager::UpdatePositionAtIndex(unsigned int index,
	glm::vec3 newPosition)
{
	EnterCriticalSection(&CSPos);
	this->vecPositions[this->m_CurrentWriteBufferIndex][index] = newPosition;
	LeaveCriticalSection(&CSPos);
	return;
}

glm::vec3 cAgentBufferManager::readPositionAtIndex(unsigned int index)
{
	EnterCriticalSection(&CSPos);
	glm::vec3 pos = this->vecPositions[this->m_CurrentReadBufferIndex][index];
	LeaveCriticalSection(&CSPos);
	return pos;
}


void cAgentBufferManager::UpdateOrientationAtIndex(unsigned int index, glm::quat ori)
{
	EnterCriticalSection(&CSBufferIndexAgent);
	this->vecOrientations[this->m_CurrentWriteBufferIndex][index] = ori;
	LeaveCriticalSection(&CSBufferIndexAgent);
	return;
}


glm::quat cAgentBufferManager::readOriAtIndex(unsigned int index)
{
	EnterCriticalSection(&CSBufferIndexAgent);
	glm::quat ori = this->vecOrientations[this->m_CurrentReadBufferIndex][index];
	LeaveCriticalSection(&CSBufferIndexAgent);
	return ori;
}
