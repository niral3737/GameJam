#pragma once
#include <physics/interfaces/physics_interfaces.h>
#include "btBulletDynamicsCommon.h"
namespace nPhysics
{
	/*class iBulletShapeInterface : public virtual iShape
	{
	public:
		virtual ~iBulletShapeInterface() {}
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}
	protected:
		iBulletShapeInterface();
		iBulletShapeInterface(const iBulletShapeInterface& other) :iShape(other) {};
		iBulletShapeInterface& operator=(const iBulletShapeInterface& other) { return *this; }
		
		btCollisionShape* mBulletShape;
	};*/

	class cBulletSphereShape : public iSphereShape
	{
	public:
		cBulletSphereShape(float radius);
		virtual ~cBulletSphereShape();
		virtual float GetRadius();
		btCollisionShape* GetBulletShape();
	protected:
		cBulletSphereShape& operator=(const cBulletSphereShape& other) { return *this; }
	private:
		btCollisionShape* mBulletShape;
	};

	class cBulletPlaneShape : public iPlaneShape
	{
	public:
		cBulletPlaneShape(float planeConstant, glm::vec3 planeNormal);
		virtual ~cBulletPlaneShape();
		float GetPlaneConstant();
		glm::vec3 GetPlaneNormal();
		btCollisionShape* GetBulletShape();
	protected:
		cBulletPlaneShape& operator=(const cBulletPlaneShape& other) { return *this; }
	private:
		btCollisionShape* mBulletShape;
	};

	class cBulletBoxShape : public iBoxShape
	{
	public:
		cBulletBoxShape(const glm::vec3& halfExtendts);
		virtual ~cBulletBoxShape();
		virtual void GetBoxHalfExtents(glm::vec3& halfEntentsOut);
		btCollisionShape* GetBulletShape();
	protected:
		cBulletBoxShape& operator=(const cBulletBoxShape& other) { return *this; }
	private:
		btCollisionShape* mBulletShape;
	};

	class cBulletCylinderShape : public iCylinderShape
	{
	public:
		cBulletCylinderShape(const glm::vec3& halfExtents);
		virtual ~cBulletCylinderShape();
		virtual float GetRadius();
		virtual int GetAxis();
		btCollisionShape* GetBulletShape();
	protected:
		cBulletCylinderShape& operator=(const cBulletCylinderShape& other) { return *this; }
	private:
		btCollisionShape* mBulletShape;
	};

	class cBulletConeShape : public iConeShape
	{
	public:
		cBulletConeShape(float radius, float height);
		virtual ~cBulletConeShape();
		virtual float GetRadius();
		virtual float GetHeight();
		btCollisionShape* GetBulletShape();
	protected:
		cBulletConeShape& operator=(const cBulletConeShape& other) { return *this; }
	private:
		btCollisionShape* mBulletShape;
	};
}

