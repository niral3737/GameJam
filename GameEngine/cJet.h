#ifndef _C_PLANE_HG_
#define _C_PLANE_HG_

#include "iEntity.h"
#include "cMeshObject.h"
#include <glad/glad.h>


class cJet :
	public iEntity
{
public:
	cJet();
	virtual ~cJet();
	cMeshObject* getMesh();
	void setMesh(iMeshObject* mesh);

	virtual std::string getName();
	std::vector<glm::vec4> collisionPointsModel;
	std::vector<glm::vec4> collisionPointsWorld;
	iMeshObject* mesh;

	float collisionPointRadius;
	bool showCollisionPoints;
	void drawCollisionPoints(GLuint shaderProgramID);
	void applyTransformationsToCollisionPoints();
	void showCollisionPoint(unsigned int pointIndex, GLuint shaderProgramID);
};


#endif // !_C_PLANE_HG_


