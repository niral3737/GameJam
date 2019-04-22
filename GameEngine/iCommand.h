#ifndef _COMMAND_HG_
#define _COMMAND_HG_

#include <vector>
#include "json.hpp"
#include "cMeshObject.h"
#include "smoothstep_Wikipedia.h"

class iCommand
{
public:
	virtual ~iCommand() {};
	
	// Since this is an interface, this initialize would have to 
	//	be common for all the commands... 
	virtual void initialize(nlohmann::json values) = 0;

	// These are both called every frame 
	virtual void update(double deltaTime) = 0;
	virtual bool isFinished(void) = 0;
};

#endif // !_COMMAND_HG_

