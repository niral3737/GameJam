#include "cBulletPhysicsWorld.h"
#include "nConvert.h"
#include "cBulletRigidBody.h"
#include "bullet_constraints.h"
#include <physics/interfaces/iConstraint.h>

namespace nPhysics{
	cBulletPhysicsWorld::cBulletPhysicsWorld()
		:mDebugDraw(0)
	{
		mCollisionConfiguration = new btDefaultCollisionConfiguration();

		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

		mOverlappingPairCache = new btDbvtBroadphase();

		mSolver = new btSequentialImpulseConstraintSolver;

		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);

		mDynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	}

	cBulletPhysicsWorld::~cBulletPhysicsWorld()
	{
		if (mDynamicsWorld)
		{
			delete mDynamicsWorld;
			mDynamicsWorld = 0;
		}

		if (mSolver)
		{
			delete mSolver;
			mSolver = 0;
		}

		if (mOverlappingPairCache)
		{
			delete mOverlappingPairCache;
			mOverlappingPairCache = 0;
		}

		if (mDispatcher)
		{
			delete mDispatcher;
			mDispatcher = 0;
		}

		if (mCollisionConfiguration)
		{
			delete mCollisionConfiguration;
			mCollisionConfiguration = 0;
		}

		if (mDebugDraw)
		{
			delete mDebugDraw;
			mDebugDraw = 0;
		}
	}
	void cBulletPhysicsWorld::SetGravity(const glm::vec3 & gravity)
	{
		mDynamicsWorld->setGravity(nConvert::ToBullet(gravity));
	}

	bool cBulletPhysicsWorld::AddBody(iRigidBody * body)
	{
		cBulletRigidBody* bulletBody = dynamic_cast<cBulletRigidBody*> (body);
		if (!bulletBody)
		{
			return false;
		}
		mDynamicsWorld->addRigidBody(bulletBody->GetBulletBody());
		return true;
	}

	bool cBulletPhysicsWorld::AddConstraint(iConstraint* constraint)
	{
		iBulletConstraint* btConstraint = dynamic_cast<iBulletConstraint*> (constraint);
		if (!btConstraint)
		{
			return false;
		}
		mDynamicsWorld->addConstraint(btConstraint->GetTypedConstraint());
		return true;
	}

	bool cBulletPhysicsWorld::RemoveBody(iRigidBody * body)
	{
		cBulletRigidBody* bulletBody = dynamic_cast<cBulletRigidBody*> (body);
		if (!bulletBody)
		{
			return false;
		}
		btRigidBody* bulletBtBody = bulletBody->GetBulletBody();
		mDynamicsWorld->removeRigidBody(bulletBtBody);
		return true;
	}

	void cBulletPhysicsWorld::Update(float dt)
	{
		mDynamicsWorld->stepSimulation(dt, 10);
	}

	void cBulletPhysicsWorld::SetDebugRenderer(iDebugRenderer * debugRenderer)
	{
		if (mDebugDraw)
		{
			mDynamicsWorld->setDebugDrawer(0);
			delete mDebugDraw;
			mDebugDraw = 0;
		}
		if (debugRenderer)
		{
			mDebugDraw = new cBulletDebugDraw(debugRenderer);
			mDynamicsWorld->setDebugDrawer(mDebugDraw);
		}
	}

	void cBulletPhysicsWorld::DrawDebug()
	{
		if (mDebugDraw)
		{
			mDynamicsWorld->debugDrawWorld();
		}
	}
}