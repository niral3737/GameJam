#include "bullet_constraints.h"
#include "nConvert.h"


namespace nPhysics
{
	cBulletDOF6Constraint::cBulletDOF6Constraint(cBulletRigidBody* rb, const btVector3& degrees)
		: iBulletConstraint()
		, iConstraint(DOF6)
	{	
		mConstraint = new btGeneric6DofConstraint(*rb->GetBulletBody(), btTransform::getIdentity(), true);
	
		mConstraint->setLinearLowerLimit(-degrees);
		mConstraint->setLinearUpperLimit(degrees);
	}
	cBulletDOF6Constraint::~cBulletDOF6Constraint()
	{

	}

	iRigidBody* cBulletDOF6Constraint::GetRigidBodyA()
	{
		return 0;
	}
	iRigidBody* cBulletDOF6Constraint::GetRigidBodyB()
	{
		return 0;
	}

	cBulletHingeConstraint::cBulletHingeConstraint(cBulletRigidBody* rb, const btVector3& PivotInA, const btVector3& AxisInA)
		: iBulletConstraint()
		, iConstraint(HINGE)
	{
		mConstraint = new btHingeConstraint(*rb->GetBulletBody(), PivotInA, AxisInA, true);
	}

	cBulletHingeConstraint::~cBulletHingeConstraint()
	{

	}

	iRigidBody* cBulletHingeConstraint::GetRigidBodyA()
	{
		return 0;
	}
	iRigidBody* cBulletHingeConstraint::GetRigidBodyB()
	{
		return 0;
	}

	cBulletP2PConstraint::cBulletP2PConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, const btVector3& PivotInA, const btVector3& PivotInB)
		: iBulletConstraint()
		, iConstraint(P2P)
	{
		mConstraint = new btPoint2PointConstraint(*rbA->GetBulletBody(), *rbB->GetBulletBody(), PivotInA, PivotInB);
	}

	cBulletP2PConstraint::~cBulletP2PConstraint()
	{

	}

	iRigidBody* cBulletP2PConstraint::GetRigidBodyA()
	{
		return 0;
	}
	iRigidBody* cBulletP2PConstraint::GetRigidBodyB()
	{
		return 0;
	}

	cBulletSliderConstraint::cBulletSliderConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, float UpperLinLimit, float LowerLinLimit )
		: iBulletConstraint()
		, iConstraint(SLIDER)
	{
		mConstraint = new btSliderConstraint(*rbA->GetBulletBody(), *rbB->GetBulletBody(), btTransform::getIdentity(), btTransform::getIdentity(), true);
	
		mConstraint->setUpperLinLimit(UpperLinLimit);
		mConstraint->setLowerLinLimit(LowerLinLimit);
	}

	cBulletSliderConstraint::~cBulletSliderConstraint()
	{

	}

	iRigidBody* cBulletSliderConstraint::GetRigidBodyA()
	{
		return 0;
	}
	iRigidBody* cBulletSliderConstraint::GetRigidBodyB()
	{
		return 0;
	}
}