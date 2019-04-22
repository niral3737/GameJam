#pragma once
#include <physics/interfaces/physics_interfaces.h>
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "cBulletDebugDraw.h"

namespace nPhysics
{
	class cBulletPhysicsWorld : public iPhysicsWorld
	{
	public:
		cBulletPhysicsWorld();
		virtual ~cBulletPhysicsWorld();

		virtual void SetGravity(const glm::vec3& gravity);

		virtual bool AddBody(iRigidBody* body);
		virtual bool RemoveBody(iRigidBody* body);

		virtual void Update(float dt);

		virtual void SetDebugRenderer(iDebugRenderer* debugRenderer);
		virtual void DrawDebug();

		virtual bool AddConstraint(iConstraint* constraint);
		
		float mDt;
	private:

		glm::vec3 mGravity;
		std::vector<iRigidBody*> mBodies;

		iDebugRenderer* mDebugRenderer;
		//collision configuration contains default setup for memory, collision setup. 
		//Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* mCollisionConfiguration;

		//use the default collision dispatcher. 
		//For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher ;

		//btDbvtBroadphase is a good general purpose broadphase. 
		//You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;

		//the default constraint solver. 
		//For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;


		//The world
		btDiscreteDynamicsWorld* mDynamicsWorld;

		cBulletDebugDraw* mDebugDraw;
	};
}