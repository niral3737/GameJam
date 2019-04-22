#ifndef _MEDIATOR_HG_
#define _MEDIATOR_HG_

#include "iMediatorEntity.h"
#include "json.hpp"

class cMediator : public iMediatorEntity
{
public:
	cMediator();
	~cMediator();

	void LoadEntity(iEntity* equipment);
	
	iEntity* findEntityByName(std::string name);

	virtual void sendMessageToEntity(nlohmann::json message);

private:
	std::vector<iEntity*> vecEntities;
};


#endif // !_MEDIATOR_HG_



