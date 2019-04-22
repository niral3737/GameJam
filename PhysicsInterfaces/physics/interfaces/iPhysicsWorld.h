#pragma once

#include "iRigidBody.h"
#include "iDubugRenderer.h"
#include "iConstraint.h"

namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {};

		virtual void SetGravity(const glm::vec3& gravity) = 0;

		virtual bool AddBody(iRigidBody* body) = 0;
		virtual bool RemoveBody(iRigidBody* body) = 0;

		virtual bool AddConstraint(iConstraint* constraint) = 0;
		
		virtual void Update(float dt) = 0;

		virtual void SetDebugRenderer(iDebugRenderer* debugRenderer) = 0;
		virtual void DrawDebug() = 0;
	};
}