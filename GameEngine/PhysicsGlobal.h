#pragma once
#include <physics/interfaces/iPhysicsFactory.h>
#include <physics/interfaces/sRigidBodyDef.h>
#include <physics/interfaces/iShape.h>
#include <physics/interfaces/iConstraint.h>
#include "json.hpp"

extern nPhysics::iPhysicsFactory* gPhysicsFactory;
extern std::vector<nPhysics::iRigidBody*> gRigidBodies;
extern std::vector<nPhysics::iConstraint*> gConstraints;

bool SetUpPhysics();
bool InitPhysics();
bool BuildRigidBodies();
bool LoadVec3(const nlohmann::json& jsonArray, glm::vec3& valueOut);
nPhysics::iShape* CreateShape(const nPhysics::sRigidBodyDef& def);