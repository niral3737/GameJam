#pragma once

#include "cMeshObject.h"
#include "iEntity.h"

class cEntity : public iEntity
{
public:
	cEntity();
	virtual ~cEntity();
	cMeshObject* mesh;
	virtual std::string getName();
	virtual cMeshObject* GetMesh();
	virtual glm::vec3 getPreviousPos();
};

