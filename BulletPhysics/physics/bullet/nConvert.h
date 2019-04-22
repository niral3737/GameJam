#pragma once
#include <physics_math.h>
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"

namespace nPhysics
{
	namespace nConvert
	{
		inline glm::vec3 ToSimple(const btVector3& vec)
		{
			return glm::vec3(vec.x(), vec.y(), vec.z());
		}

		inline void ToSimple(const btVector3& vecIn, glm::vec3& vecOut)
		{
			vecOut.x = vecIn.x();
			vecOut.y = vecIn.y();
			vecOut.z = vecIn.z();
		}

		inline btVector3 ToBullet(const glm::vec3& vec)
		{
			return btVector3(vec.x, vec.y, vec.z);
		}

		inline void ToBullet(const glm::vec3& vecIn, btVector3& vecOut)
		{
			vecOut.setValue(vecIn.x, vecIn.y, vecIn.z);
		}

		inline glm::mat4 ToSimple(const btTransform& transformIn)
		{
			glm::mat4 mat;
			transformIn.getOpenGLMatrix(&mat[0][0]);
			return mat;
		}

		inline void ToSimple(const btTransform& transformIn, glm::mat4& matOut)
		{
			transformIn.getOpenGLMatrix(&matOut[0][0]);
		}
	}
}