#ifndef _COMMAND_GROUP_HG_
#define _COMMAND_GROUP_HG_

#include "iCommand.h"
#include <vector>

class cCommandGroup
{
public:
	cCommandGroup();

	virtual void Update(double deltaTime);
	virtual bool isFinished(void);

	std::vector<iCommand*> vecCommands;
	std::vector<cCommandGroup*> vecCommandGroups;
	std::string friendlyName;
	bool pause;
	cCommandGroup* findCommandGroupByFrinedlyName(std::string friendlyName);
private:
	bool bIsFinished;
};

#endif // !_COMMAND_GROUP_HG_



