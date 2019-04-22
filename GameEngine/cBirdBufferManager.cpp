#include "cBirdBufferManager.h"
#include <process.h>
#include <Windows.h>
#include <iostream>

CRITICAL_SECTION CSBufferIndex;

cBirdBufferManager::cBirdBufferManager()
{
	InitializeCriticalSection(&CSBufferIndex);

	this->m_CurrentWriteBufferIndex = 0;
	this->m_CurrentReadBufferIndex = 1;

	return;
}

cBirdBufferManager::~cBirdBufferManager()
{
	DeleteCriticalSection(&CSBufferIndex);

	return;
}

void cBirdBufferManager::InitBuffers(unsigned int size)
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

void cBirdBufferManager::SwitchBuffers(void)
{
	EnterCriticalSection(&CSBufferIndex);

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

	LeaveCriticalSection(&CSBufferIndex);
	return;
}

unsigned int cBirdBufferManager::getCurrentReadBuffer(void)
{
	return this->m_CurrentReadBufferIndex;
}

unsigned int cBirdBufferManager::getCurrentWriteBuffer(void)
{
	return this->m_CurrentWriteBufferIndex;
}

void cBirdBufferManager::UpdatePositionAtIndex(unsigned int index,
	glm::vec3 newPosition)
{
	//std::cout << "index " << index << std::endl;
	//std::cout << " write index " << this->m_CurrentWriteBufferIndex << std::endl;
	EnterCriticalSection(&CSBufferIndex);
	this->vecPositions[this->m_CurrentWriteBufferIndex][index] = newPosition;
	LeaveCriticalSection(&CSBufferIndex);
	return;
}