#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "cMeshObject.h"
#include "cModelDrawInfo.h"
#include "cVAOMeshUtils.h"
#include "cCannonBall.h"
#include "cBase.h" 
#include "cAABB.h"

class cPhysics
{
public:
	void PhysicsUpdate(double deltaTime, GLint program);

	void PlayerPhysicsUpdate(cMeshObject* playerObject, double deltaTime);

	void CalculateClosestPointsOnMesh(cAABB* aabb, glm::vec3 pointToTest, std::vector<glm::vec3> &vecPoints);

	unsigned int FindClosestPointOfAll(const glm::vec3& pointToTest, const std::vector<glm::vec3>& vecPoints, glm::vec3& finalPoint);

	void GetClosestTriangle(unsigned int finalIndex, const cModelDrawInfo& info, glm::vec3& closestTriangle);

	bool SphereSphereCollision(cMeshObject* pA, cMeshObject* pB);

	bool SphereCubeCollision(cMeshObject* pSphere, cMeshObject* pCube, cModelDrawInfo& meshInfo);

	void addProjectileAim(double deltaTime, cCannonBall* cannonBall, cBase* shootingBase);

	glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

	static const glm::vec3 ACCEL_GRAVITY;

	static float initialProjectileVelocity;

	glm::vec3 adjustHeight(cMeshObject* object, GLint program);
	void drawPath(GLint program);
};

#endif // !_cPhysics_HG_