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

namespace nPhysics
{
	class iDebugRenderer
	{
	public:
		virtual ~iDebugRenderer() {}

		virtual void RenderLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) = 0;
		virtual void RenderBox(const glm::vec3& extents, const glm::mat4& trans, const glm::vec3& color) = 0;
		virtual void RenderSphere(const glm::mat4& transform, float radius, const glm::vec3& color) = 0;
		virtual void RenderPlane(const glm::vec3& planeNormal, float planceConst, const glm::mat4& transform, const glm::vec3& color) = 0;
		virtual void RenderCone(float radius, float height, int upAxis, const glm::mat4& transForm, const glm::vec3& color) = 0;
		/*virtual void DrawSphere(const glm::vec3& position, float radius) = 0;

		virtual void DrawPlane(const glm::vec3& normal, float constant) = 0;*/

		//virtual void DrawLine(const glm::vec3& pointA, const glm::vec3& pointB) = 0;
		//virtual void DrawCone(const glm::vec3& point, const glm::vec3& direction) = 0;
	};
}
