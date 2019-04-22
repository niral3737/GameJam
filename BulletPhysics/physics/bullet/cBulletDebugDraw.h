#pragma once

#include "LinearMath/btIDebugDraw.h"
#include <physics/interfaces/iDubugRenderer.h>

namespace nPhysics
{
	class cBulletDebugDraw : public btIDebugDraw
	{
	public:
		cBulletDebugDraw(iDebugRenderer* debugRenderer);
		~cBulletDebugDraw();

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
		virtual void reportErrorWarning(const char* warningString);
		virtual void draw3dText(const btVector3& location, const char* textString);
		virtual void setDebugMode(int debugMode);
		virtual int getDebugMode() const;

	private:
		iDebugRenderer* mDebugRenderer;
	};
}