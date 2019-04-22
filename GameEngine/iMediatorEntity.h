#ifndef _MEDIATOR_ENTITY_HG_
#define _MEDIATOR_ENTITY_HG_

#include "iEntity.h"
#include <string>
#include <vector>
#include "json.hpp"

class iMediatorEntity
{
public:
	virtual ~iMediatorEntity() {};

	virtual iEntity* findEntityByName(std::string name) = 0;

	virtual void sendMessageToEntity(nlohmann::json message) = 0;
};

#endif // !_MEDIATOR_ENTITY_HG_