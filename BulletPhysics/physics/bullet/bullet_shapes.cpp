#include "bullet_shapes.h"
#include "nConvert.h"

namespace nPhysics
{
	cBulletSphereShape::cBulletSphereShape(float radius)
	{
		mBulletShape = new btSphereShape(btScalar(radius));
	}
	cBulletSphereShape::~cBulletSphereShape()
	{
		delete mBulletShape;
		mBulletShape = 0;
	}
	float cBulletSphereShape::GetRadius()
	{
		return dynamic_cast<btSphereShape*>(mBulletShape)->getRadius();
	}

	btCollisionShape* cBulletSphereShape::GetBulletShape()
	{
		return mBulletShape;
	}

	btCollisionShape* cBulletPlaneShape::GetBulletShape()
	{
		return mBulletShape;
	}
	
	cBulletPlaneShape::cBulletPlaneShape(float planeConstant, glm::vec3 planeNormal)
	{
		mBulletShape = new btStaticPlaneShape(nConvert::ToBullet(planeNormal), btScalar(0.0f));
	}

	cBulletPlaneShape::~cBulletPlaneShape()
	{
		delete mBulletShape;
		mBulletShape = 0;
	}

	float cBulletPlaneShape::GetPlaneConstant()
	{
		return dynamic_cast<btStaticPlaneShape*>(mBulletShape)->getPlaneConstant();
	}

	glm::vec3 cBulletPlaneShape::GetPlaneNormal()
	{
		return nConvert::ToSimple(dynamic_cast<btStaticPlaneShape*>(mBulletShape)->getPlaneNormal());
	}

	cBulletBoxShape::cBulletBoxShape(const glm::vec3 & halfExtendts)
	{
		mBulletShape = new btBoxShape(nConvert::ToBullet(halfExtendts));
	}

	cBulletBoxShape::~cBulletBoxShape()
	{}

	void cBulletBoxShape::GetBoxHalfExtents(glm::vec3 & halfEntentsOut)
	{
		nConvert::ToSimple(dynamic_cast<btBoxShape*> (mBulletShape)->getHalfExtentsWithoutMargin(), halfEntentsOut);
	}

	btCollisionShape * cBulletBoxShape::GetBulletShape()
	{
		return mBulletShape;
	}

	cBulletCylinderShape::cBulletCylinderShape(const glm::vec3 & halfExtents)
	{
		btCylinderShape* shape = new btCylinderShape(nConvert::ToBullet(halfExtents));
		mBulletShape = shape;
	}

	cBulletCylinderShape::~cBulletCylinderShape()
	{}

	float cBulletCylinderShape::GetRadius()
	{
		return dynamic_cast<btCylinderShape*>(mBulletShape)->getRadius();
	}

	int cBulletCylinderShape::GetAxis()
	{
		return dynamic_cast<btCylinderShape*>(mBulletShape)->getUpAxis();
	}

	btCollisionShape * cBulletCylinderShape::GetBulletShape()
	{
		return mBulletShape;
	}


	cBulletConeShape::cBulletConeShape(float radius, float height)
	{
		btConeShape* shape = new btConeShape(radius, height);
		mBulletShape = shape;
	}
	cBulletConeShape::~cBulletConeShape()
	{
	}
	float cBulletConeShape::GetRadius()
	{
		return dynamic_cast<btConeShape*>(mBulletShape)->getRadius();
	}
	float cBulletConeShape::GetHeight()
	{
		return dynamic_cast<btConeShape*>(mBulletShape)->getHeight();
	}

	btCollisionShape * cBulletConeShape::GetBulletShape()
	{
		return mBulletShape;
	}

}