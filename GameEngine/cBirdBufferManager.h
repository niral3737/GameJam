#pragma once

#include <glm/vec3.hpp>
#include <vector>

class cBirdBufferManager
{
public:
	cBirdBufferManager();
	~cBirdBufferManager();

	void InitBuffers(unsigned int size);
	// Called by the renderer (or main thread)
	void SwitchBuffers(void);

	unsigned int getCurrentReadBuffer(void);
	unsigned int getCurrentWriteBuffer(void);

	// This is called by each of the Daleks
	void UpdatePositionAtIndex(unsigned int index,
		glm::vec3 newPosition);

	// Here is the triple buffer
	// (an array of 3 vectors)
	static const unsigned int NUMBEROFBUFFERS = 3;
	std::vector<glm::vec3> vecPositions[NUMBEROFBUFFERS];

private:
	unsigned int m_CurrentReadBufferIndex;
	unsigned int m_CurrentWriteBufferIndex;
};