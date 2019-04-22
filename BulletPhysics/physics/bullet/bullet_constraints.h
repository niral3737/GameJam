#pragma once

#include <physics/interfaces/iConstraint.h>
#include <physics/interfaces/constraints.h>
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "cBulletRigidBody.h"

namespace nPhysics
{
	class cBulletPhysicsWorld;

	class iBulletConstraint : public virtual iConstraint
	{
	public:
		iBulletConstraint() : iConstraint(DOF6) {}
		~iBulletConstraint() {}
		virtual btTypedConstraint* GetTypedConstraint() = 0;
	protected:
		iBulletConstraint(const iBulletConstraint& other): iConstraint(other) {}
		iBulletConstraint& operator=(const iBulletConstraint& other) { return *this; }
	};

	class cBulletDOF6Constraint : public iDOF6Constraint , public iBulletConstraint
	{
	public:
		cBulletDOF6Constraint(cBulletRigidBody* rb, const btVector3& degrees);
		virtual ~cBulletDOF6Constraint();

		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();

		virtual btTypedConstraint* GetTypedConstraint() { return mConstraint; }
	protected:
		btGeneric6DofConstraint* mConstraint;
		cBulletDOF6Constraint(const cBulletDOF6Constraint& other) :iConstraint(other) {};
		cBulletDOF6Constraint& operator=(const cBulletDOF6Constraint& other) { return *this; }
	};

	class cBulletHingeConstraint : public iHingeConstraint, public iBulletConstraint
	{
	public:
		cBulletHingeConstraint(cBulletRigidBody* rb, const btVector3& PivotInA, const btVector3& AxisInA);
		virtual ~cBulletHingeConstraint();

		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();

		virtual btTypedConstraint* GetTypedConstraint() { return mConstraint; }
	protected:
		btHingeConstraint* mConstraint;
		cBulletHingeConstraint(const cBulletHingeConstraint& other) :iConstraint(other) {};
		cBulletHingeConstraint& operator=(const cBulletHingeConstraint& other) { return *this; }
	};

	class cBulletP2PConstraint : public iP2PConstraint, public iBulletConstraint
	{
	public:
		cBulletP2PConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, const btVector3& PivotInA, const btVector3& PivotInB);
		virtual ~cBulletP2PConstraint();

		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();

		virtual btTypedConstraint* GetTypedConstraint() { return mConstraint; }
	protected:
		btPoint2PointConstraint* mConstraint;
		cBulletP2PConstraint(const cBulletP2PConstraint& other) :iConstraint(other) {};
		cBulletP2PConstraint& operator=(const cBulletP2PConstraint& other) { return *this; }
	};

	class cBulletSliderConstraint : public iSliderConstraint, public iBulletConstraint
	{
	public:
		cBulletSliderConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, float UpperLinLimit, float LowerLinLimit);
		virtual ~cBulletSliderConstraint();

		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();

		virtual btTypedConstraint* GetTypedConstraint() { return mConstraint; }
	protected:
		btSliderConstraint* mConstraint;
		cBulletSliderConstraint(const cBulletSliderConstraint& other) :iConstraint(other) {};
		cBulletSliderConstraint& operator=(const cBulletSliderConstraint& other) { return *this; }
	};
}