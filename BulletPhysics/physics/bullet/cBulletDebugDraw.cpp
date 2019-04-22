#include "cBulletDebugDraw.h"
#include "nConvert.h"

namespace nPhysics
{
	cBulletDebugDraw::cBulletDebugDraw(iDebugRenderer * debugRenderer)
		: mDebugRenderer(debugRenderer)
	{
	
	}
	cBulletDebugDraw::~cBulletDebugDraw()
	{}
	void cBulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
	{
		mDebugRenderer->RenderLine(nConvert::ToSimple(from), nConvert::ToSimple(to), nConvert::ToSimple(color));
	}
	void cBulletDebugDraw::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
	{}
	void cBulletDebugDraw::reportErrorWarning(const char * warningString)
	{}
	void cBulletDebugDraw::draw3dText(const btVector3 & location, const char * textString)
	{}
	void cBulletDebugDraw::setDebugMode(int debugMode)
	{
		
	}
	int cBulletDebugDraw::getDebugMode() const
	{
		return 1; //TODO 
	}
}

