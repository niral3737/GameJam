#include "physicsGlobal.h"
#include <Windows.h>
#include "json.hpp"
#include <fstream>

nPhysics::iPhysicsFactory* gPhysicsFactory = 0;
typedef nPhysics::iPhysicsFactory*(*f_CreateFactory)();
std::vector<nPhysics::iRigidBody*> gRigidBodies;
std::vector<nPhysics::iConstraint*> gConstraints;


bool SetUpPhysics()
{
	InitPhysics();
	BuildRigidBodies();
	return true;
}

bool InitPhysics()
{
	if (gPhysicsFactory)
	{
		return false;
	}

	HINSTANCE hGetProcDll = LoadLibraryA("BulletPhysics.dll");
	if (!hGetProcDll)
	{
		return false;
	}

	f_CreateFactory CreateFactory = (f_CreateFactory)GetProcAddress(hGetProcDll, "CreateFactory");
	if (!CreateFactory)
	{
		return false;
	}

	gPhysicsFactory = CreateFactory();
	return true;
}

bool BuildRigidBodies()
{
	std::ifstream jsonFile("physics_config.json");
	nlohmann::json jsonConfig = nlohmann::json::parse(jsonFile);
	jsonFile.close();
	nlohmann::json jsonRigidBodyDefs = jsonConfig["RigidBodyDefs"];

	size_t numDefs = jsonRigidBodyDefs.size();
	for (size_t c = 0; c < numDefs; c++)
	{
		nlohmann::json jsonRigidBodyDef = jsonRigidBodyDefs[c];
		if (jsonRigidBodyDef.is_object())
		{
			nPhysics::sRigidBodyDef def;
			if (jsonRigidBodyDef["ShapeType"].is_string())
			{
				def.ShapeType = jsonRigidBodyDef["ShapeType"].get<std::string>();
			}
			def.Mass = jsonRigidBodyDef["Mass"].get<float>();
			LoadVec3(jsonRigidBodyDef["Position"], def.Position);
			LoadVec3(jsonRigidBodyDef["Velocity"], def.Velocity);
			LoadVec3(jsonRigidBodyDef["Orientation"], def.Orientation);

			if (def.ShapeType == "Sphere")
			{
				def.Radius = jsonRigidBodyDef["Radius"].get<float>();
			}
			else if (def.ShapeType == "Plane")
			{
				def.PlaneConstant = jsonRigidBodyDef["PlaneConstant"].get<float>();
				LoadVec3(jsonRigidBodyDef["PlaneNormal"], def.PlaneNormal);
			}
			else if (def.ShapeType == "Box")
			{
				LoadVec3(jsonRigidBodyDef["HalfExtents"], def.HalfExtents);
			}
			else if (def.ShapeType == "Cone")
			{
				def.Radius = jsonRigidBodyDef["Radius"].get<float>();
				def.Height = jsonRigidBodyDef["Height"].get<float>();
			}
			else if (def.ShapeType == "Cylinder")
			{
				LoadVec3(jsonRigidBodyDef["HalfExtents"], def.HalfExtents);
			}

			gRigidBodies.push_back(gPhysicsFactory->CreateRigidBody(
				def,
				CreateShape(def)
			));
		}
	}
	return true;
}

bool LoadVec3(const nlohmann::json& jsonArray, glm::vec3& valueOut)
{
	if (jsonArray.is_array())
	{
		valueOut.x = jsonArray[0];
		valueOut.y = jsonArray[1];
		valueOut.z = jsonArray[2];
		return true;
	}
	return false;
}

nPhysics::iShape* CreateShape(const nPhysics::sRigidBodyDef& def)
{
	nPhysics::iShape* shape = 0;
	if (def.ShapeType == "Sphere")
	{
		shape = gPhysicsFactory->CreateSphereShape(def.Radius);
	}
	else if (def.ShapeType == "Plane")
	{
		shape = gPhysicsFactory->CreatePlaneShape(def.PlaneNormal, def.PlaneConstant);
	}
	else if (def.ShapeType == "Box")
	{
		shape = gPhysicsFactory->CreateBoxShape(def.HalfExtents);
	}
	else if (def.ShapeType == "Cone")
	{
		shape = gPhysicsFactory->CreateConeShape(def.Radius, def.Height);
	}
	else if (def.ShapeType == "Cylinder")
	{
		shape = gPhysicsFactory->CreateCylinderShape(def.HalfExtents);
	}
	return shape;
}