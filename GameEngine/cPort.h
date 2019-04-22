#ifndef _C_PORT_HG_
#define _C_PORT_HG_

#include "iEntity.h"
#include "cShip.h"
#include "cTreasure.h"
#include "iMediatorEntity.h"
#include "iMeshObject.h"
#include "cMeshObject.h"

class cPort :
	public iEntity
{
public:
	cPort();
	virtual ~cPort();
	virtual void setMediator(iMediatorEntity* mediator);

	cShip* ship;
	cTreasure* treasureGold;
	cTreasure* treasureGemStones;

	bool isShipDocked;
	
	// using cMeshObject* instead of iMeshObject* just for ease of operation
	cMeshObject* getMesh();
	void setMesh(iMeshObject* mesh);
	virtual std::string getName();
	void convertGemStonesToGold();

	iMeshObject* mesh;
private:
	iMediatorEntity* mediator;
};

#endif // !_C_PORT_HG_



