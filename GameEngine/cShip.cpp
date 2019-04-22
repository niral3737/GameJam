#include "cShip.h"
#include "cPort.h"
#include "cRandomHelper.h"
#include <iostream>

cShip::cShip()
{
	eShipState = UNKNONE;
	timeToWait = 0.0f;
	timeWaited = 0.0f;
	this->mesh = NULL;
	this->percentGoldToSpendWaiting = 3;
}

cShip::~cShip()
{}

void cShip::setMediator(iMediatorEntity * mediator)
{
	this->mediator = mediator;
}

cMeshObject* cShip::getMesh()
{
	return (cMeshObject*) mesh;
}

void cShip::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;

}

std::string cShip::getName()
{
	if (this->mesh != NULL)
	{
		return this->getMesh()->friendlyName;
	}
	return "";
}

//TODO improve this by sending messages back and forth
void cShip::attackEmptyPort(std::string portFriendlyName)
{
	cPort* portUnderAttack = (cPort*) this->mediator->findEntityByName(portFriendlyName);
	
	//transection start
	//25% gold
	unsigned int goldToBeDeducted = portUnderAttack->treasureGold->numberOfTreasure / 4;
	unsigned int gemStonesToBeDeducted = portUnderAttack->treasureGemStones->numberOfTreasure / 4;

	//edge case where division returns 0
	if (goldToBeDeducted == 0)
	{
		goldToBeDeducted = portUnderAttack->treasureGold->numberOfTreasure;
	}

	if (gemStonesToBeDeducted == 0)
	{
		gemStonesToBeDeducted = portUnderAttack->treasureGemStones->numberOfTreasure;
	}

	if (goldToBeDeducted > getTreasureCapacity())
	{
		goldToBeDeducted = getTreasureCapacity();
	}

	if (gemStonesToBeDeducted > getTreasureCapacity())
	{
		gemStonesToBeDeducted = getTreasureCapacity();
	}

	portUnderAttack->treasureGold->deductTreasure(goldToBeDeducted);
	portUnderAttack->treasureGemStones->deductTreasure(gemStonesToBeDeducted);

	cPort* ownPort = (cPort*)this->mediator->findEntityByName(ownPortName);

	ownPort->treasureGold->addTreasure(goldToBeDeducted);
	ownPort->treasureGemStones->addTreasure(gemStonesToBeDeducted);
	//transection end

	//ship->treasureGold->
}

void cShip::attackPortWithShip(std::string portFriendlyName)
{
	cPort* portDefending = (cPort*)this->mediator->findEntityByName(portFriendlyName);
	cPort* ownPort = (cPort*)this->mediator->findEntityByName(ownPortName);


	unsigned int ownPortGold = ownPort->treasureGold->numberOfTreasure;
	unsigned int defendingPortGold = portDefending->treasureGold->numberOfTreasure;

	//<alteration
	if (ownPort->ship->isSuperShip)
	{
		ownPortGold *= 1.5;
	}

	if (portDefending->ship->isSuperShip)
	{
		defendingPortGold *= 1.5;
	}
	//alteration>

	cPort* smallerNumOfGoldPort;
	cPort* largerNumOfGoldPort;
	if (ownPortGold < defendingPortGold)
	{
		smallerNumOfGoldPort = ownPort;
		largerNumOfGoldPort = portDefending;
	}
	else
	{
		smallerNumOfGoldPort = portDefending;
		largerNumOfGoldPort = ownPort;
	}

	unsigned int randomNumber = cRandomHelper::generateRandomIntInRange(0, ownPortGold + defendingPortGold);


	cPort* winnerPort;
	cPort* looserPort;

	//<alteration
	unsigned int smallerGold = smallerNumOfGoldPort->treasureGold->numberOfTreasure;
	if (smallerNumOfGoldPort->ship->isSuperShip)
	{
		smallerGold = smallerNumOfGoldPort->treasureGold->numberOfTreasure * 1.5;
	}
	//alteration>

	if (randomNumber < smallerGold)
	{
		//smallerNumOfGoldPort wins
		winnerPort = smallerNumOfGoldPort;
		looserPort = largerNumOfGoldPort;
		
	}
	else
	{
		//largerNumOfGoldPort wins
		winnerPort = largerNumOfGoldPort;
		looserPort = smallerNumOfGoldPort;
	}

	//transetion starts

	//50% gold
	unsigned int goldToBeDeducted = looserPort->treasureGold->numberOfTreasure / 2;
	unsigned int gemsToBeDeducted = looserPort->treasureGemStones->numberOfTreasure / 2;

	//edge case 1/2 will give 0 and will not deduct anything from port
	if (goldToBeDeducted == 0)
	{
		goldToBeDeducted = looserPort->treasureGold->numberOfTreasure;
	}
	if (gemsToBeDeducted == 0)
	{
		gemsToBeDeducted = looserPort->treasureGemStones->numberOfTreasure;
	}

	if (goldToBeDeducted > getTreasureCapacity())
	{
		goldToBeDeducted = getTreasureCapacity();
	}

	if (gemsToBeDeducted > getTreasureCapacity())
	{
		gemsToBeDeducted = getTreasureCapacity();
	}

	std::cout << "**********************************" << std::endl;
	std::cout << "(" << ownPortGold << "," << defendingPortGold << ")" << std::endl;
	std::cout << "(" << ownPort->getName() << ", " << portDefending->getName() << ")" << std::endl;
	std::cout << "hasSuperShip: (" << ownPort->ship->isSuperShip << ", " << portDefending->ship->isSuperShip << ")" << std::endl;
	std::cout << "random number : " << randomNumber << std::endl;
	std::cout << "(winner, looser)" << std::endl;
	std::cout << "(" << winnerPort->getName() << ", " << looserPort->getName() << ")" << std::endl;
	std::cout << "Gold to be deducted: " << goldToBeDeducted << std::endl;
	
	winnerPort->treasureGold->addTreasure(goldToBeDeducted);
	winnerPort->treasureGemStones->addTreasure(gemsToBeDeducted);
	looserPort->treasureGold->deductTreasure(goldToBeDeducted);
	looserPort->treasureGemStones->deductTreasure(gemsToBeDeducted);

	std::cout << "after deduction" << std::endl;
	std::cout << "(" << winnerPort->treasureGold->numberOfTreasure << ", " << looserPort->treasureGold->numberOfTreasure << ")" << std::endl;
	std::cout << "**********************************" << std::endl;
	//transetion ends
}

void cShip::spendGoldWaiting()
{
	cPort* ownPort = (cPort*)this->mediator->findEntityByName(ownPortName);
	unsigned int ownGold = ownPort->treasureGold->numberOfTreasure;
	
	std::cout << "before spend waiting " << ownGold << std::endl;
	std::cout << "percent Gold to deduct " << percentGoldToSpendWaiting << std::endl;
	std::cout << "Who's spendin? : " << ownPort->getName() << std::endl;
	ownPort->treasureGold->numberOfTreasure = (ownGold * (100 - percentGoldToSpendWaiting)) / 100;
	std::cout << "after spend waiting " << ownPort->treasureGold->numberOfTreasure << std::endl;
}

unsigned int cShip::getTreasureCapacity()
{
	return this->treasureCapacity;
}

void cShip::upgradeToSuperShip()
{
	this->isSuperShip = true;
	this->treasureCapacity = 75;
	this->percentGoldToSpendWaiting = 5;
	this->getMesh()->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
}
