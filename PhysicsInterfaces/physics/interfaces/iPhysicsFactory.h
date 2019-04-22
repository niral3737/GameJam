#pragma once

#include "sRigidBodyDef.h"
#include "iShape.h"
#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "shapes.h"
#include "constraints.h"

namespace nPhysics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {};
		virtual iPhysicsWorld* CreatePhysicsWorld() = 0;
		virtual iRigidBody* CreateRigidBody(const sRigidBodyDef& def, iShape* shape) = 0;
		virtual iSphereShape* CreateSphereShape(float radius) = 0;
		virtual iPlaneShape* CreatePlaneShape(const glm::vec3& normal, float constant) = 0;
		virtual iBoxShape* CreateBoxShape(const glm::vec3& halfExtents) = 0;
		virtual iConeShape* CreateConeShape(float radius, float height) = 0;
		virtual iCylinderShape* CreateCylinderShape(const glm::vec3& halfExtents) = 0;
		virtual iDOF6Constraint* CreateDOF6Constraint(iRigidBody* rbA, const glm::vec3& degrees) = 0;
		virtual iHingeConstraint* CreateHingeConstraint(iRigidBody* rbA, const glm::vec3& PivotInA, const glm::vec3& AxisInA) = 0;
		virtual iP2PConstraint* CreateP2PConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& PivotInA, const glm::vec3& PivotInB) = 0;
		virtual iSliderConstraint* CreateSliderConstraint(iRigidBody* rbA, iRigidBody* rbB, float UpperLinLimt, float LowerLinLimit) = 0;
		
	};
}
