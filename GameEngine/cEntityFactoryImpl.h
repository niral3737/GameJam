#ifndef _ENTITY_FACTORY_IMPL_HG_
#define _ENTITY_FACTORY_IMPL_HG_

#include "iEntity.h" 
#include <string>
#include "iMediatorEntity.h"

class cEntityFactoryImpl
{
public:
	cEntityFactoryImpl();
	~cEntityFactoryImpl();

	bool isEntityTypeValid(std::string entityType);

	iEntity* createEntity(std::string entityType);

	void setMediator(iMediatorEntity* mediator);
private:
	iMediatorEntity* mediator;
};

#endif // !_ENTITY_FACTORY_IMPL_HG_



