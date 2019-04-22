#pragma once

#include <glm/vec3.hpp>
#include <string>

namespace nPhysics
{
	struct sRigidBodyDef
	{
		sRigidBodyDef() : 
			Mass(0.0f),
			Position(glm::vec3(0.0f)),
			Velocity(glm::vec3(0.0f)),
			Orientation(glm::vec3(0.0f)),
			Radius(1.0f),
			PlaneConstant(0.0f),
			PlaneNormal(glm::vec3(0.0f)),
			HalfExtents(glm::vec3(0.0f)),
			Height(0.0f)
		{
		}
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 Orientation; //Euler Angles
		float Mass;

		//Shape Information
		std::string ShapeType;
		float Radius;
		float PlaneConstant;
		glm::vec3 PlaneNormal;
		glm::vec3 HalfExtents;
		float Height;
	};
}