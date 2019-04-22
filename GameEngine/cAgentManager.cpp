#include "cAgentManager.h"
#include "cRandThreaded.h"

cAgentManager::cAgentManager()
{}

cAgentManager::~cAgentManager()
{}

void cAgentManager::init()
{
	bufferManager = new cAgentBufferManager();
	bufferManager->InitPosBuffer(NUM_AGENTS, glm::vec3(0.0f));
	bufferManager->InitOriBuffer(NUM_AGENTS, glm::quat(glm::vec3(0.0f)));

	seekAgent = new cAgentThreaded();
	seekAgent->behaviourType = cAgentThreaded::eAgentBehaviourType::SEEK;
	seekAgent->setBufferManager(bufferManager);
	seekAgent->setIndex(0);

	fleeAgent = new cAgentThreaded();
	fleeAgent->behaviourType = cAgentThreaded::eAgentBehaviourType::FLEE;
	fleeAgent->setBufferManager(bufferManager);
	fleeAgent->setIndex(1);

	wanderAgent = new cAgentThreaded();
	wanderAgent->behaviourType = cAgentThreaded::eAgentBehaviourType::WANDER;
	wanderAgent->setBufferManager(bufferManager);
	wanderAgent->setIndex(2);
}

void cAgentManager::start()
{
	{
		cRandThreaded* pRT = new cRandThreaded();
		seekAgent->CreateThreadAndStartUpdating(0, bufferManager, pRT);
	}
	{
		cRandThreaded* pRT = new cRandThreaded();
		fleeAgent->CreateThreadAndStartUpdating(1, bufferManager, pRT);
	}
	{
		cRandThreaded* pRT = new cRandThreaded();
		wanderAgent->CreateThreadAndStartUpdating(2, bufferManager, pRT);
	}
}
