#include "cCommandGroup.h"

cCommandGroup::cCommandGroup()
{
	this->bIsFinished = false;
	this->pause = false;
	this->friendlyName = "";
}

void cCommandGroup::Update(double deltaTime)
{
	if (this->pause)
	{
		return;
	}
	// Assume everything is done...
	bool bAllCommandsAreFinished = true;
	bool bAllCommandGroupsAreFinished = true;

	// Run ALL the "commands" at the same time (in parallel)
	for (std::vector<iCommand*>::iterator itCurCom = this->vecCommands.begin();
		itCurCom != this->vecCommands.end(); itCurCom++)
	{
		iCommand* pCurCom = *itCurCom;

		if (!pCurCom->isFinished())
		{
			pCurCom->update(deltaTime);

			// At least one command is still running...
			bAllCommandsAreFinished = false;
		}
	}

	// Find the 1st command group that is NOT done and run that
	for (std::vector<cCommandGroup*>::iterator itCurComGrp = this->vecCommandGroups.begin();
		itCurComGrp != this->vecCommandGroups.end(); itCurComGrp++)
	{
		cCommandGroup* pCurCmdGrp = *itCurComGrp;

		// This one NOT running? 
		if (!pCurCmdGrp->isFinished())
		{
			// Yes, so update...
			pCurCmdGrp->Update(deltaTime);

			bAllCommandGroupsAreFinished = false;
			// ... and quit the loop
			break;
		}
	}

	if (bAllCommandsAreFinished && bAllCommandGroupsAreFinished)
	{
		this->bIsFinished = true;
	}

	// Run the command groups one at a time
}

bool cCommandGroup::isFinished(void)
{
	return this->bIsFinished;
}

cCommandGroup* cCommandGroup::findCommandGroupByFrinedlyName(std::string friendlyName)
{
	for (size_t i = 0; i < vecCommandGroups.size(); i++)
	{
		if (vecCommandGroups[i]->friendlyName == friendlyName)
		{
			return vecCommandGroups[i];
		}
	}
	return NULL;
}
