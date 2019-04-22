#ifndef _C_TREASURE_HG_
#define _C_TREASURE_HG_

#include "iEntity.h"
class cTreasure :
	public iEntity
{
public:
	cTreasure();
	virtual ~cTreasure();

	unsigned int numberOfTreasure;
	unsigned int valueOfOneTreasure;
	
	void addTreasure(unsigned int valueToAdd);
	void deductTreasure(unsigned int valueToDeduct);
	virtual std::string getName();

	unsigned int getTotalTreasure();
};

#endif // !_C_TREASURE_HG_




