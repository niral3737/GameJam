#ifndef _IEQUIPMENT_HG_
#define _IEQUIPMENT_HG_

#include "iMeshObject.h"

class iEquipment
{
public:
	virtual ~iEquipment() {};

	virtual void move(void) = 0;
	virtual iMeshObject* getMesh() = 0;
	virtual void setMesh(iMeshObject* mesh) = 0;
};

#endif // !_IEQUIPMENT_HG_
