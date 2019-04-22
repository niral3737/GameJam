#pragma once

#include <glm/vec3.hpp>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

class cAgentBufferManager
{
public:
	cAgentBufferManager();
	~cAgentBufferManager();

	void InitBuffers(unsigned int size);
	void InitPosBuffer(unsigned int size, glm::vec3 initPos);
	void InitOriBuffer(unsigned int size, glm::quat initOrientation);
	// Called by the renderer (or main thread)
	void SwitchBuffers(void);

	unsigned int getCurrentReadBuffer(void);
	unsigned int getCurrentWriteBuffer(void);

	// This is called by each of the Daleks
	void UpdatePositionAtIndex(unsigned int index,
		glm::vec3 newPosition);

	void UpdateOrientationAtIndex(unsigned int index, glm::quat ori);
	
	glm::vec3 readPositionAtIndex(unsigned int index);
	glm::quat readOriAtIndex(unsigned int index);

	// Here is the triple buffer
	// (an array of 3 vectors)
	static const unsigned int NUMBEROFBUFFERS = 3;
	std::vector<glm::vec3> vecPositions[NUMBEROFBUFFERS];
	std::vector<glm::quat> vecOrientations[NUMBEROFBUFFERS];

private:
	unsigned int m_CurrentReadBufferIndex;
	unsigned int m_CurrentWriteBufferIndex;
};