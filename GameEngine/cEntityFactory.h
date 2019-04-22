#ifndef _ENTITY_FACTORY_HG_
#define _ENTITY_FACTORY_HG_

#include "iEntity.h" 
#include "cEntityFactoryImpl.h"
#include "iMediatorEntity.h"
#include <string>

class cEntityFactory
{
public:
	cEntityFactory();
	cEntityFactory(iMediatorEntity* mediator);
	~cEntityFactory();

	bool isEntityTypeValid(std::string entityType);

	iEntity* createEntity(std::string entityType);
private:
	cEntityFactoryImpl* impl;
};

#endif // !_ENTITY_FACTORY_HG_



