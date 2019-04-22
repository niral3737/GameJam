#include "cEntityFactory.h"

#include "cEntityFactoryImpl.h"

cEntityFactory::cEntityFactory() 
	: impl(new cEntityFactoryImpl)
{}

cEntityFactory::cEntityFactory(iMediatorEntity* mediator)
	: impl(new cEntityFactoryImpl())
{
	impl->setMediator(mediator);
}

cEntityFactory::~cEntityFactory()
{
	delete this->impl;
}

bool cEntityFactory::isEntityTypeValid(std::string entityType)
{
	return this->impl->isEntityTypeValid(entityType);
}

iEntity * cEntityFactory::createEntity(std::string entityType)
{
	return this->impl->createEntity(entityType);
}
