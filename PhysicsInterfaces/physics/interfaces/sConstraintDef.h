#pragma once
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/ext/scalar_constants.hpp"
#include <string>

namespace nPhysics
{
	struct sConstraintDef
	{
		sConstraintDef()
			: Degrees(glm::vec3(0.0f))
			, RbAIndex(0)
			, RbBIndex(0)
			, PivotInA(glm::vec3(0.0f))
			, AxisInA(glm::vec3(0.0f))
			, PivotInB(glm::vec3(0.0f))
			, UpperLinLimit(0.0f)
			, LowerLinLimit(0.0f)
		{}

		std::string Type;
		glm::vec3 Degrees;
		size_t RbAIndex;
		size_t RbBIndex;
		glm::vec3 PivotInA;
		glm::vec3 PivotInB;
		glm::vec3 AxisInA;
		float UpperLinLimit;
		float LowerLinLimit;
	};
}