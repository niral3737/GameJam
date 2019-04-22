
#include "cEntityFactoryImpl.h"

#include "cPort.h"
#include "cShip.h"
#include "cNormalShip.h"
#include "cSuperShip.h"
#include "cGoldCoins.h"
#include "cGemStones.h"
#include "cBase.h"
#include "cIsland.h"
#include "cCannonBall.h"
#include "cJet.h"

cEntityFactoryImpl::cEntityFactoryImpl()
{}


cEntityFactoryImpl::~cEntityFactoryImpl()
{}

bool cEntityFactoryImpl::isEntityTypeValid(std::string entityType)
{
	return entityType == "port" 
		|| entityType == "normalShip" 
		|| entityType == "superShip"
		|| entityType == "goldCoins" 
		|| entityType == "gemStones"
		|| entityType == "base"
		|| entityType == "island"
		|| entityType == "cannonBall"
		|| entityType == "jet";
}

iEntity * cEntityFactoryImpl::createEntity(std::string entityType)
{
	iEntity* entity = NULL;

	if (!isEntityTypeValid(entityType))
	{
		return entity;
	}

	return entity;
}

void cEntityFactoryImpl::setMediator(iMediatorEntity * mediator)
{
	this->mediator = mediator;
}
