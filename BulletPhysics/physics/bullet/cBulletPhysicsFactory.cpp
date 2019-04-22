#include "cBulletPhysicsFactory.h"
#include "cBulletRigidBody.h"
#include "bullet_shapes.h"
#include "cBulletPhysicsWorld.h"
#include "bullet_constraints.h"
#include "nConvert.h"

namespace nPhysics
{
	cBulletPhysicFactory::~cBulletPhysicFactory()
	{}
	iPhysicsWorld * cBulletPhysicFactory::CreatePhysicsWorld()
	{
		return new cBulletPhysicsWorld();
	}
	iRigidBody * cBulletPhysicFactory::CreateRigidBody(const sRigidBodyDef & def, iShape * shape)
	{
		return new cBulletRigidBody(def, shape);
	}
	iSphereShape * cBulletPhysicFactory::CreateSphereShape(float radius)
	{
		return new cBulletSphereShape(radius);
	}
	iPlaneShape * cBulletPhysicFactory::CreatePlaneShape(const glm::vec3 & normal, float constant)
	{
		return new cBulletPlaneShape(constant, normal);
	}

	iBoxShape* cBulletPhysicFactory::CreateBoxShape(const glm::vec3& halfExtents)
	{
		return new cBulletBoxShape(halfExtents);
	}

	iConeShape* cBulletPhysicFactory::CreateConeShape(float radius, float height)
	{
		return new cBulletConeShape(radius, height);
	}

	iCylinderShape* cBulletPhysicFactory::CreateCylinderShape(const glm::vec3& halfExtents)
	{
		return new cBulletCylinderShape(halfExtents);
	}

	iDOF6Constraint* cBulletPhysicFactory::CreateDOF6Constraint(iRigidBody* rbA, const glm::vec3& degrees)
	{
		cBulletRigidBody* bulletRB = dynamic_cast<cBulletRigidBody*>(rbA);
		if (!bulletRB)
		{
			return 0;
		}

		cBulletDOF6Constraint* constraint = new cBulletDOF6Constraint(bulletRB, nConvert::ToBullet(degrees));
		return constraint;
	}

	iHingeConstraint* cBulletPhysicFactory::CreateHingeConstraint(iRigidBody* rbA, const glm::vec3& PivotInA, const glm::vec3& AxisInA)
	{
		cBulletRigidBody* bulletRB = dynamic_cast<cBulletRigidBody*>(rbA);
		if (!bulletRB)
		{
			return 0;
		}

		cBulletHingeConstraint* constraint = new cBulletHingeConstraint(bulletRB, nConvert::ToBullet(PivotInA), nConvert::ToBullet(AxisInA));
		return constraint;
	}

	iP2PConstraint* cBulletPhysicFactory::CreateP2PConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& PivotInA, const glm::vec3& PivotInB)
	{
		cBulletRigidBody* bulletRBA = dynamic_cast<cBulletRigidBody*>(rbA);
		if (!bulletRBA)
		{
			return 0;
		}

		cBulletRigidBody* bulletRBB = dynamic_cast<cBulletRigidBody*>(rbB);
		if (!bulletRBB)
		{
			return 0;
		}

		cBulletP2PConstraint* constraint = new cBulletP2PConstraint(bulletRBA, bulletRBB, nConvert::ToBullet(PivotInA), nConvert::ToBullet(PivotInB));
		return constraint;
	}

	iSliderConstraint* cBulletPhysicFactory::CreateSliderConstraint(iRigidBody* rbA, iRigidBody* rbB, float UpperLinLimt, float LowerLinLimit)
	{
		cBulletRigidBody* bulletRBA = dynamic_cast<cBulletRigidBody*>(rbA);
		if (!bulletRBA)
		{
			return 0;
		}

		cBulletRigidBody* bulletRBB = dynamic_cast<cBulletRigidBody*>(rbB);
		if (!bulletRBB)
		{
			return 0;
		}

		cBulletSliderConstraint* constraint = new cBulletSliderConstraint(bulletRBA, bulletRBB, UpperLinLimt, LowerLinLimit);
		return constraint;
	}


}