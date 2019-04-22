#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "iShape.h"
#include <string>

namespace nPhysics
{
	class iRigidBody
	{
	public:
		virtual ~iRigidBody() {};

		virtual void GetTransform(glm::mat4& transformOut) = 0;
		virtual glm::vec3 GetPosition() = 0;
		virtual glm::vec3 GetPreviousPosition() = 0;
		virtual glm::vec3 GetVelocity() = 0;
		virtual glm::vec3 GetAcceleration() = 0;
		virtual std::string GetFriendlyName() = 0;
		virtual float GetMass() = 0;
		virtual void SetPosition(glm::vec3 position) = 0;
		virtual void SetPreviousPosition(glm::vec3 previousPosition) = 0;
		virtual void SetVelocity(glm::vec3 velocity) = 0;
		virtual void SetAcceleration(glm::vec3 acceleration) = 0;
		virtual void SetFriendlyName(std::string friendlyName) = 0;
		virtual iShape* GetShape() = 0;
	protected:
		iRigidBody(){}
		iRigidBody(const iRigidBody& other) {}
		iRigidBody& operator=(const iRigidBody* other) { return *this; }
	};
}