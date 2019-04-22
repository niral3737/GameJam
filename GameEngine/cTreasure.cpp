#include "cTreasure.h"



cTreasure::cTreasure()
{
	this->numberOfTreasure = 0;
}


cTreasure::~cTreasure()
{}

void cTreasure::addTreasure(unsigned int valueToAdd)
{
	this->numberOfTreasure += valueToAdd;
}

void cTreasure::deductTreasure(unsigned int valueToDeduct)
{
	this->numberOfTreasure -= valueToDeduct;
}

std::string cTreasure::getName()
{
	//should not be invoked
	// there's no name to return;
	return "";
}

unsigned int cTreasure::getTotalTreasure()
{
	return numberOfTreasure * valueOfOneTreasure;
}
