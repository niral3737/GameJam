#pragma once
#include "cAgentThreaded.h"
#include "cAgentBufferManager.h"
//#include "cMeshObject.h"

class cAgentManager
{
public:
	cAgentManager();
	~cAgentManager();
	void init();
	void start();
	cAgentBufferManager* bufferManager;
	cAgentThreaded* seekAgent;
	cAgentThreaded* fleeAgent;
	cAgentThreaded* wanderAgent;
	//cMeshObject* seekAgentMesh;
private:
	const int NUM_AGENTS = 10;
	
	
};