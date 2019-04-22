#ifndef _SHIP_HG_
#define _SHIP_HG_

#include "iEntity.h"
#include "iMediatorEntity.h"
#include "iMeshObject.h"
#include "cMeshObject.h"
#include "cTreasure.h"

enum shipState
{
	UNKNONE,
	WAITING,
	GOING,
	ATTACKING,
	DEAD
};

//base class for all ships
class cShip :
	public iEntity
{
public:
	cShip();
	virtual ~cShip();
	virtual void setMediator(iMediatorEntity* mediator);
	
	cMeshObject* getMesh();
	void setMesh(iMeshObject* mesh);

	size_t portToGoIndex;
	std::string ownPortName;
	bool isSuperShip;

	shipState eShipState;

	float timeToWait;
	float timeWaited;
	unsigned int percentGoldToSpendWaiting;

	iMeshObject* mesh;
	virtual std::string getName();
	unsigned int treasureCapacity;

	void attackEmptyPort(std::string portFriendlyName);
	void attackPortWithShip(std::string portFriendlyName);
	void spendGoldWaiting();
	unsigned int getTreasureCapacity();
	void upgradeToSuperShip();
private:
	iMediatorEntity* mediator;
};

#endif // !_SHIP_HG_



