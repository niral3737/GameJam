#pragma once

#include "iShape.h"
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace nPhysics
{
	class iSphereShape : public iShape
	{
	public:
		virtual ~iSphereShape() {}
		virtual float GetRadius() = 0;
	protected:
		iSphereShape(): iShape(SHAPE_TYPE_SPHERE){}
		iSphereShape(const iSphereShape& other) : iShape(other) {}
		iSphereShape& operator=(const iSphereShape& other) { return *this; }
	};

	class iPlaneShape : public iShape
	{
	public:
		virtual ~iPlaneShape() {}
		virtual float GetPlaneConstant() = 0;
		virtual glm::vec3 GetPlaneNormal() = 0;
	protected:
		iPlaneShape() : iShape(SHAPE_TYPE_PLANE) {}
		iPlaneShape(const iPlaneShape& other) : iShape(other) {}
		iPlaneShape& operator=(const iPlaneShape& other) { return *this; }
	};

	//BOX
	class iBoxShape : public iShape
	{
	public:
		virtual ~iBoxShape(){}
		virtual void GetBoxHalfExtents(glm::vec3& halfExtentsOut) = 0;
	protected:
		iBoxShape() : iShape(SHAPE_TYPE_BOX) {} 
		iBoxShape(const iBoxShape& other) : iShape(other) {}
		iBoxShape& operator=(const iBoxShape& other) { return *this; }
	};

	//cylinder
	class iCylinderShape : public iShape
	{
	public:
		virtual ~iCylinderShape() {}
		virtual float GetRadius() = 0;
		virtual int GetAxis() = 0;
	protected:
		iCylinderShape() : iShape(SHAPE_TYPE_CYLINDER) {}
		iCylinderShape(const iCylinderShape& other) : iShape(other) {}
		iCylinderShape& operator=(const iCylinderShape& other) { return *this; }
	};

	//cone
	class iConeShape : public iShape
	{
	public:
		virtual ~iConeShape() {}
		virtual float GetRadius() = 0;
		virtual float GetHeight() = 0;
	protected:
		iConeShape() : iShape(SHAPE_TYPE_CONE) {}
		iConeShape(const iConeShape& other) : iShape(other) {}
		iConeShape& operator=(const iConeShape& other) { return *this; }
	};
}