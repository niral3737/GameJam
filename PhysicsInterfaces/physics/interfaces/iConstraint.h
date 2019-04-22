#pragma once
#include "eConstraintType.h"
#include "iRigidBody.h"

namespace nPhysics
{

	class iConstraint
	{
	public:
		virtual ~iConstraint(){}

		virtual iRigidBody* GetRigidBodyA() = 0;
		virtual iRigidBody* GetRigidBodyB() = 0;

		inline eConstraintType GetConstraintType() const { return mConstraintType; }
	protected:
		iConstraint(eConstraintType constraintType) : mConstraintType(constraintType) {}
		iConstraint(const iConstraint& other) {}
		iConstraint& operator=(const iConstraint& other) { return *this; }
	private:
		eConstraintType mConstraintType;
	};
}