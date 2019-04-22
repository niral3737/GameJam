#ifndef _ENTITY_HG_
#define _ENTITY_HG_

#include <string>
#include "cMeshObject.h"

class iEntity
{
public:
	virtual ~iEntity() {};
	virtual std::string getName() = 0;
	virtual cMeshObject* GetMesh() = 0;
	virtual glm::vec3 getPreviousPos() = 0;
};


#endif // !ENTITY_HG_
