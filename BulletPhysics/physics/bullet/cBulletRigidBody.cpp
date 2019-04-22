#include "cBulletRigidBody.h"
#include "nConvert.h"
#include "bullet_shapes.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"

namespace nPhysics
{
	cBulletRigidBody::cBulletRigidBody(const sRigidBodyDef & def , iShape* shape)
	{
		mShape = shape;
		btCollisionShape* colShape;
		if (def.ShapeType == "Sphere")
		{
			colShape = ((cBulletSphereShape*)shape)->GetBulletShape();
		}
		else if (def.ShapeType == "Plane")
		{
			colShape = ((cBulletPlaneShape*)shape)->GetBulletShape();
		}
		else if (def.ShapeType == "Box")
		{
			colShape = ((cBulletBoxShape*)shape)->GetBulletShape();
		}
		else if (def.ShapeType == "Cone")
		{
			colShape = ((cBulletConeShape*)shape)->GetBulletShape();
		}
		else if (def.ShapeType == "Cylinder")
		{
			colShape = ((cBulletCylinderShape*)shape)->GetBulletShape();

		}
		
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(def.Mass);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);

		if (isDynamic)
		{
			colShape->calculateLocalInertia(mass, localInertia);
		}

		startTransform.setOrigin(nConvert::ToBullet(def.Position));
		mMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
		mBody = new btRigidBody(rbInfo);
		mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));
		// mBody->setCollisionFlags(mBody->getCollisionFlags() | btCollisionObject::CF_HAS_COLLISION_SOUND_TRIGGER);
		if (def.ShapeType == "Cylinder")
		{
			mBody->setAngularFactor(btScalar(0.0f));
		}
	
	}

	cBulletRigidBody::~cBulletRigidBody()
	{
		delete mBody;
		mBody = 0;
		delete mMotionState;
		mMotionState = 0;
	}

	void cBulletRigidBody::GetTransform(glm::mat4 & transformOut)
	{
		btTransform transform;
		mMotionState->getWorldTransform(transform);
		nConvert::ToSimple(transform, transformOut);
	}

	glm::vec3 cBulletRigidBody::GetPosition()
	{	
		glm::mat4 transform;
		GetTransform(transform);
		return transform[3];
	}

	glm::vec3 cBulletRigidBody::GetPreviousPosition()
	{
		return glm::vec3();
	}

	glm::vec3 cBulletRigidBody::GetVelocity()
	{
		return nConvert::ToSimple(mBody->getLinearVelocity());
	}

	glm::vec3 cBulletRigidBody::GetAcceleration()
	{
		return glm::vec3();
	}

	std::string cBulletRigidBody::GetFriendlyName()
	{
		return friendlyName;
	}

	float cBulletRigidBody::GetMass()
	{
		return 0.0f;
	}

	void cBulletRigidBody::SetPosition(glm::vec3 position)
	{
		//mBody->
		btTransform transform;
		//mMotionState->getWorldTransform(transform);
		//transform = mBody->getWorldTransform();
		transform.setIdentity();
		transform.setOrigin(nConvert::ToBullet(position));
		mBody->setWorldTransform(transform);
		//mBody->getMotionState()->setWorldTransform(transform);
		mMotionState->setWorldTransform(transform);
	}

	void cBulletRigidBody::SetPreviousPosition(glm::vec3 previousPosition)
	{}

	void cBulletRigidBody::SetVelocity(glm::vec3 velocity)
	{
		mBody->activate(true);
		mBody->setLinearVelocity(nConvert::ToBullet(velocity));
	}

	void cBulletRigidBody::SetAcceleration(glm::vec3 acceleration)
	{}

	void cBulletRigidBody::SetFriendlyName(std::string friendlyName)
	{
		this->friendlyName = friendlyName;
	}


	iShape * cBulletRigidBody::GetShape()
	{
		return mShape;
	}
}