#pragma once

#include <physics/interfaces/physics_interfaces.h>
#include "btBulletDynamicsCommon.h"

namespace nPhysics
{
	class cBulletRigidBody : public iRigidBody
	{
	public:
		cBulletRigidBody(const sRigidBodyDef& def, iShape* shape);
		 ~cBulletRigidBody();

		 void GetTransform(glm::mat4& transformOut) ;
		 glm::vec3 GetPosition() ;
		 glm::vec3 GetPreviousPosition() ;
		 glm::vec3 GetVelocity() ;
		 glm::vec3 GetAcceleration() ;
		 virtual std::string GetFriendlyName();
		 float GetMass() ;
		 void SetPosition(glm::vec3 position) ;
		 void SetPreviousPosition(glm::vec3 previousPosition) ;
		 void SetVelocity(glm::vec3 velocity) ;
		 void SetAcceleration(glm::vec3 acceleration) ;
		 virtual void SetFriendlyName(std::string friendlyName);
		 iShape* GetShape();

		 inline btRigidBody* GetBulletBody()
		 {
			 return mBody;
		 }
	protected:
		cBulletRigidBody(const cBulletRigidBody& other) : iRigidBody(other) {}
		cBulletRigidBody& operator=(const cBulletRigidBody* other) { return *this; }
	
	private:
		btDefaultMotionState* mMotionState;
		btRigidBody* mBody;
		//btCollisionShape* mShape;
		iShape* mShape;
		std::string friendlyName;
	};
}
