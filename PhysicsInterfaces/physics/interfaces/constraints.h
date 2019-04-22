#pragma once

#include "iConstraint.h"
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
	class iDOF6Constraint : public virtual iConstraint
	{
	public:
		virtual ~iDOF6Constraint(){}
	protected:
		iDOF6Constraint() : iConstraint(DOF6) {}
		iDOF6Constraint(const iDOF6Constraint& other) : iConstraint(other) {}
		iDOF6Constraint& operator=(const iDOF6Constraint& other) { return *this; }
	};

	class iHingeConstraint : public virtual iConstraint
	{
	public:
		virtual ~iHingeConstraint() {}
	protected:
		iHingeConstraint() : iConstraint(HINGE) {}
		iHingeConstraint(const iHingeConstraint& other) : iConstraint(other) {}
		iHingeConstraint& operator=(const iHingeConstraint& other) { return *this; }
	};

	class iP2PConstraint : public virtual iConstraint
	{
	public:
		virtual ~iP2PConstraint() {}
	protected:
		iP2PConstraint() : iConstraint(P2P) {}
		iP2PConstraint(const iP2PConstraint& other) : iConstraint(other) {}
		iP2PConstraint& operator=(const iP2PConstraint& other) { return *this; }
	};

	class iSliderConstraint : public virtual iConstraint
	{
	public:
		virtual ~iSliderConstraint() {}
	protected:
		iSliderConstraint() : iConstraint(SLIDER) {}
		iSliderConstraint(const iSliderConstraint& other) : iConstraint(other) {}
		iSliderConstraint& operator=(const iSliderConstraint& other) { return *this; }
	};
}