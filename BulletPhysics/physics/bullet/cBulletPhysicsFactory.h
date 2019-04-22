#pragma once
#include <physics/interfaces/physics_interfaces.h>

namespace nPhysics
{
	class cBulletPhysicFactory : public iPhysicsFactory
	{
		virtual ~cBulletPhysicFactory();
		virtual iPhysicsWorld* CreatePhysicsWorld();
		virtual iRigidBody* CreateRigidBody(const sRigidBodyDef& def, iShape* shape);
		virtual iSphereShape* CreateSphereShape(float radius);
		virtual iPlaneShape* CreatePlaneShape(const glm::vec3& normal, float constant);
		virtual iBoxShape* CreateBoxShape(const glm::vec3& halfExtents);
		virtual iConeShape* CreateConeShape(float radius, float height);
		virtual iCylinderShape* CreateCylinderShape(const glm::vec3& halfExtents);
		virtual iDOF6Constraint* CreateDOF6Constraint(iRigidBody* rbA, const glm::vec3& degrees);
		virtual iHingeConstraint* CreateHingeConstraint(iRigidBody* rbA, const glm::vec3& PivotInA, const glm::vec3& AxisInA);
		virtual iP2PConstraint* CreateP2PConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& PivotInA, const glm::vec3& PivotInB);
		virtual iSliderConstraint* CreateSliderConstraint(iRigidBody* rbA, iRigidBody* rbB, float UpperLinLimt, float LowerLinLimit);
	};
}