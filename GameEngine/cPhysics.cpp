// Physics
//#include "globalStuff.h"

#include "cPhysics.h"
#include "cSceneUtils.h"
#include "cCamera.h"
#include "cShaderUtils.h"
#include "cJsonUtils.h"
#include "cBase.h" 
#include "cCannonBall.h" 
#include "cRandomHelper.h"
#include "cJet.h"
#include "cAABB.h"

const glm::vec3 cPhysics::ACCEL_GRAVITY = glm::vec3(0.0f, -5.0f, 0.0f);
float const INITIAL_Y_VELOCITY = 10.0f;

float cPhysics::initialProjectileVelocity = 10.0f;

void cPhysics::PhysicsUpdate(double deltaTime, GLint program)
{
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	//// BOX LOCK
	//const float LIMIT_POS_X = collisionTest.max.x;			// Lowest the objects can go
	//const float LIMIT_NEG_X = collisionTest.min.x;			// Lowest the objects can go
	//const float LIMIT_POS_Y = collisionTest.min.y + collisionTest.extent.x;			// Lowest the objects can go 
	//const float LIMIT_NEG_Y = collisionTest.min.y;			// Lowest the objects can go
	//const float LIMIT_POS_Z = collisionTest.max.z;			// Lowest the objects can go
	//const float LIMIT_NEG_Z = collisionTest.min.z;			// Lowest the objects can go

	// The longest deltatime to wait before another update
	//const double largestDeltaTime = 0.10; // 10Hz

	//if (deltaTime > largestDeltaTime)
	//{
	//	deltaTime = largestDeltaTime;
	//}

	//cMeshObject* ship = (cMeshObject*) sceneUtils->findObjectByFriendlyName("ship");
	//cMeshObject* house = (cMeshObject*)sceneUtils->findObjectByFriendlyName("house");

	//glm::vec3 shipPointBellow = adjustHeight(ship, program);
	//glm::vec3 housePointBellow = adjustHeight(house, program);

	//cMeshObject* sphere = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("sphere3");
	//sphere->isVisible = true;
	//sphere->isWireFrame = true;
	//sphere->dontLight = true;
	//sphere->scale = 1.0f / 8.0f;

	//glm::mat4 matIdentity = glm::mat4(1.0f);

	//sphere->materialDiffuse = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//sphere->position = shipPointBellow;
	//cSceneUtils::getInstance()->drawObject(sphere, matIdentity, program);

	//sphere->position = housePointBellow;
	//cSceneUtils::getInstance()->drawObject(sphere, matIdentity, program);

	//sphere->isVisible = false;
	//

	//drawPath(program);
	//jet->getMesh()->velocity += jet->getMesh()->acceleration * (float) deltaTime;
	//jet->getMesh()->position += jet->getMesh()->velocity * (float) deltaTime;



	//jet->applyTransformationsToCollisionPoints();
	//for (size_t i = 0; i < jet->collisionPointsWorld.size(); i++)
	//{
	//	glm::vec3 currentPoint = jet->collisionPointsWorld[i];
	//	cAABB* aabb =  sceneUtils->terrainHierarchy->mapAABBs[cAABB::generateId(jet->collisionPointsWorld[i], 10.0f)];
	//	if (aabb == nullptr)
	//	{
	//		//std::cout << "AABB Not Found for " << cAABB::generateId(jet->collisionPointsWorld[i], 10.0f) << std::endl;
	//		return;
	//	}
	//	std::vector<glm::vec3> vecPoints;
	//	CalculateClosestPointsOnMesh(aabb, jet->collisionPointsWorld[i], vecPoints);

	//	glm::vec3 pointToTest = jet->collisionPointsWorld[i];
	//	glm::vec3 closestPoint;
	//	unsigned int triIndex = FindClosestPointOfAll(jet->collisionPointsWorld[i], vecPoints, closestPoint);

	//	cAABB::sAABBTriangle triToTest = aabb->vecTriangles[triIndex];
	//	float distance = glm::distance(closestPoint, pointToTest);
	//	
	//	if (distance <= jet->collisionPointRadius)
	//	{
	//		std::string collidedOn = "";
	//		// colided
	//		// collisionPointsWorld[0] = Left Wing 
	//		// collisionPointsWorld[1] = Right Wing 
	//		// collisionPointsWorld[2] = Nose 
	//		switch (i)
	//		{
	//		case 0:
	//			collidedOn = "Left Wing";
	//			jet->getMesh()->velocity.x = 0.0f;
	//			break;
	//		case 1:
	//			collidedOn = "Right Wing";
	//			jet->getMesh()->velocity.x = 0.0f;
	//			break;
	//		case 2:
	//			collidedOn = "Nose";
	//			jet->getMesh()->velocity.z = 0.0f;
	//			break;
	//		default:
	//			break;
	//		}
	//		jet->showCollisionPoint(i, program);
	//		jet->getMesh()->velocity.y = 0.0f;
	//		std::cout << "We have collision on " << collidedOn << std::endl;
	//	}
	//}


	//std::vector<glm::vec3> vecPoints;

	//CalculateClosestPointsOnMesh(modelDrawInfo, currentObject->position, vecPoints);
	//glm::vec3 closestPoint;
	//unsigned int triIndex = FindClosestPointOfAll(currentObject->position, vecPoints, closestPoint);
	//glm::ivec3 CurTri = modelDrawInfo.pTriangles[triIndex];

	//// ... call the ClosestPointToTriangle...
	//// normal to one of it's corner
	//glm::vec3 normal = glm::cross(modelDrawInfo.pVerticesFromFile[CurTri.y] - modelDrawInfo.pVerticesFromFile[CurTri.x],
	//	modelDrawInfo.pVerticesFromFile[CurTri.z] - modelDrawInfo.pVerticesFromFile[CurTri.x]);

	//float distance = glm::distance(closestPoint, currentObject->position);
	//if (distance <= ((sSphere*)currentObject->theShape)->radius)
	//{
	//	// colided
	//	std::cout << currentObject->friendlyName << " colided with " << objectThatCollides->friendlyName << std::endl;
	//	normal = glm::normalize(normal);
	//	currentObject->velocity = glm::reflect(currentObject->velocity, normal);
	//	currentObject->velocity.y = 0.0f;
	//}

}

glm::vec3 cPhysics::adjustHeight(cMeshObject* object, GLint program)
{
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	cAABB* aabb = sceneUtils->terrainHierarchy->mapAABBs[cAABB::generateId(object->position, 30.0f)];
	if (aabb == nullptr)
	{
		std::cout << "AABB Not Found for " << cAABB::generateId(object->position, 30.0f) << std::endl;
		object->position.y = 18.0f;
		glm::vec3 belowPoint = object->position.y - glm::vec3(0.0f, 18.0f, 0.0f);
		return belowPoint;
	}

	std::vector<glm::vec3> vecPoints;
	CalculateClosestPointsOnMesh(aabb, object->position, vecPoints);

	glm::vec3 pointToTest = object->position;
	glm::vec3 pointBelow;
	unsigned int triIndex = FindClosestPointOfAll(object->position, vecPoints, pointBelow);

	cAABB::sAABBTriangle triToTest = aabb->vecTriangles[triIndex];
	//float distance = glm::distance(closestPoint, pointToTest);
		
	//18.0f is 1/4 of the height of middle island 
	object->position.y = pointBelow.y + 18.0f;

	return pointBelow;
}

void cPhysics::drawPath(GLint program)
{
	const unsigned int NUM_OF_BALLS = 75;
	cMeshObject* ship = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("ship");
	cMeshObject* house = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("house");

	glm::vec3 direction = house->position - ship->position;
	glm::vec3 stepInDirection = direction / (float) NUM_OF_BALLS;
	glm::vec3 pathTravelled = ship->position;

	cMeshObject* sphere = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("sphere");
	sphere->isVisible = true;
	sphere->isWireFrame = true;
	sphere->dontLight = true;
	sphere->scale = 2.0f / 8.0f;

	glm::mat4 matIdentity = glm::mat4(1.0f);

	sphere->materialDiffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	for (size_t i = 0; i < NUM_OF_BALLS - 1; i++)
	{
		pathTravelled += stepInDirection;
		sphere->position = pathTravelled; 
		adjustHeight(sphere, program);
		cSceneUtils::getInstance()->drawObject(sphere, matIdentity, program);
	}
	sphere->isVisible = false;
}

void cPhysics::PlayerPhysicsUpdate(cMeshObject* playerObject, double deltaTime)
{
	// EULER INTEGRATION

	// Player ship has to have it's velocity tending to go
	// back to 0, so we need to update the values.
	double currentVel = 0.0f;
	if (playerObject->velocity.z > 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.z -= 0.5f;
		if (playerObject->acceleration.z < -10.0f)
		{
			playerObject->acceleration.z = -10.0f;
		}
		currentVel = playerObject->velocity.z + playerObject->acceleration.z * deltaTime;
		if (currentVel <= 0.0f)
		{
			playerObject->acceleration.z = 0.0f;
			playerObject->velocity.z = 0.0f;
		}
	}
	if (playerObject->velocity.z < 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.z += 0.5f;
		if (playerObject->acceleration.z > 10.0f)
		{
			playerObject->acceleration.z = 10.0f;
		}
		currentVel = playerObject->velocity.z + playerObject->acceleration.z * deltaTime;
		if (currentVel >= 0.0f)
		{
			playerObject->acceleration.z = 0.0f;
			playerObject->velocity.z = 0.0f;
		}
	}

	if (playerObject->velocity.x > 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.x -= 0.5f;
		if (playerObject->acceleration.x < -3.0f)
		{
			playerObject->acceleration.x = -3.0f;
		}
		currentVel = playerObject->velocity.x + playerObject->acceleration.x * deltaTime;
		if (currentVel <= 0.0f)
		{
			playerObject->acceleration.x = 0.0f;
			playerObject->velocity.x = 0.0f;
		}
	}
	if (playerObject->velocity.x < 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.x += 0.5f;
		if (playerObject->acceleration.x > 3.0f)
		{
			playerObject->acceleration.x = 3.0f;
		}
		currentVel = playerObject->velocity.x + playerObject->acceleration.x * deltaTime;
		if (currentVel >= 0.0f)
		{
			playerObject->acceleration.x = 0.0f;
			playerObject->velocity.x = 0.0f;
		}
	}

	// Player ship will navigate forward only, and rotate to change direction.
	playerObject->velocity.z += playerObject->acceleration.z * deltaTime;
	playerObject->velocity.x += playerObject->acceleration.x * deltaTime;
	// Clamp max velocity
	playerObject->velocity.z = playerObject->velocity.z > 5.0f ? 5.0f : playerObject->velocity.z;
	playerObject->velocity.z = playerObject->velocity.z < -5.0f ? -5.0f : playerObject->velocity.z;
	playerObject->velocity.x = playerObject->velocity.x > 5.0f ? 5.0f : playerObject->velocity.x;
	playerObject->velocity.x = playerObject->velocity.x < -5.0f ? -5.0f : playerObject->velocity.x;
	// Update position
	playerObject->position.z += playerObject->velocity.z * deltaTime;
	playerObject->postRotation.y += playerObject->velocity.x * deltaTime;
}

glm::vec3 cPhysics::ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	//   float snom = Dot(p - a, ab), sdenom = Dot(p - b, a - b);
	float snom = glm::dot(p - a, ab);
	float sdenom = glm::dot(p - b, a - b);

	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	//    float tnom = Dot(p - a, ac), tdenom = Dot(p - c, a - c);
	float tnom = glm::dot(p - a, ac);
	float tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	//    float unom = Dot(p - b, bc), udenom = Dot(p - c, b - c);
	float unom = glm::dot(p - b, bc);
	float udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


													// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
													//    Vector n = Cross(b - a, c - a);
	glm::vec3 n = glm::cross(b - a, c - a);
	//    float vc = Dot(n, Cross(a - p, b - p));

	float vc = glm::dot(n, glm::cross(a - p, b - p));

	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	//    float va = Dot(n, Cross(b - p, c - p));
	float va = glm::dot(n, glm::cross(b - p, c - p));


	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	//    float vb = Dot(n, Cross(c - p, a - p));
	float vb = glm::dot(n, glm::cross(c - p, a - p));

	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

void cPhysics::CalculateClosestPointsOnMesh(cAABB* aabb, glm::vec3 pointToTest, std::vector<glm::vec3> &vecPoints)
{
	vecPoints.clear();

	// For each triangle in the mesh information...
	for (unsigned int triIndex = 0; triIndex != aabb->vecTriangles.size(); triIndex++)
	{
		cAABB::sAABBTriangle CurTri = aabb->vecTriangles[triIndex];

		glm::vec3 vert_1 = CurTri.verts[0];
		glm::vec3 vert_2 = CurTri.verts[1];
		glm::vec3 vert_3 = CurTri.verts[2];

		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);

		vecPoints.push_back(closestPoint);
	}
}

// Find closest point from all points
unsigned int cPhysics::FindClosestPointOfAll(const glm::vec3& pointToTest, const std::vector<glm::vec3>& vecPoints, glm::vec3& finalPoint)
{
	glm::vec3 closestPoint = vecPoints[0]; // Begin assuming that the closes point is the first one
	float currentYDist = glm::abs(pointToTest.y - closestPoint.y);

	unsigned int finalIndex = 0;
	for (unsigned int index = 1; index != vecPoints.size(); index++)
	{

		float xDistance = glm::abs(pointToTest.x - vecPoints[index].x);
		float zDistance = glm::abs(pointToTest.z - vecPoints[index].z);
		float yDistance = glm::abs(pointToTest.y - vecPoints[index].y);

		if (xDistance <= 1.0f && zDistance <= 1.0f && yDistance <= currentYDist)
		{
			currentYDist = yDistance;
			finalIndex = index;
			closestPoint = vecPoints[index];
		}
		// Check next point distance
		// Should we set it to absolute?
		//float nextDist = glm::distance(pointToTest, vecPoints[index]);
		//if (nextDist < currentDist)
		//{
		//	// If the distance checked now is smaller, this is the current smallest distanced point
		//	currentDist = nextDist;
		//	// And this is then our new closest point
		//	closestPoint = vecPoints[index];
		//	finalIndex = index;
		//}
	}

	// In the end, pass the new closest point
	finalPoint = closestPoint;

	return finalIndex;
}

void cPhysics::GetClosestTriangle(unsigned int finalIndex, const cModelDrawInfo& info, glm::vec3& closestTriangle)
{
	// By logic, the closest point index is the closest triangle index.
	//int x, y, z;
	///*x = info.pTriangles[finalIndex].x;
	//y = info.pTriangles[finalIndex].y;
	//z = info.pTriangles[finalIndex].z;*/

	// Update the closest triangle
	//closestTriangle = { x, y, z };
}

void cPhysics::addProjectileAim(double deltaTime, cCannonBall* cannonBall, cBase* shootingBase)
{
//	cCamera* camera = cCamera::getInstance();
//
//	glm::vec3 cameraAt = camera->getAtInWorldSpace();
//
//	//cMeshObject* shootRing = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("ring");
//
	cMeshObject* debugSphere = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("base");
//
//	debugSphere->position = cameraAt;
//	debugSphere->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
//	debugSphere->isVisible = true;
//	debugSphere->scale = 0.1f;
//	debugSphere->isWireFrame = true;
//	debugSphere->isUpdatedByPhysics = false;
//
//	/*shootRing->position = cameraAt;
//	shootRing->setOrientationEulerAngles(glm::vec3(90.0f, 0.0f, 0.0f), true);
//	shootRing->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
//	shootRing->isVisible = true;
//	shootRing->scale = 0.01f;
//	shootRing->isWireFrame = true;
//	shootRing->isUpdatedByPhysics = false;
//*/
//	glm::mat4 matWorld = glm::mat4(1.0f);
	std::string shaderProgramName = cJsonUtils::getJsonInstance()["shaderProgramName"].get<std::string>();
//	cSceneUtils::getInstance()->drawObject(debugSphere, matWorld, cShaderUtils::getInstance()->getIdFromFriendlyName(shaderProgramName));

	//glm::vec3 projVelWorldSpace = cannonBall->lastInitialVelocity;
	glm::vec3 projVelWorldSpace = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 projPosition = shootingBase->getMesh()->position;
	glm::vec3 projAccel = ACCEL_GRAVITY;


	// draw projectile path

	float timeStep = 0.25f;
	float howLongWeGonnaRun = 10.0f;

	for (float time = 0.0f; time < howLongWeGonnaRun; time += timeStep)
	{
		projVelWorldSpace.x = projVelWorldSpace.x + (projAccel.x * timeStep);
		projVelWorldSpace.y = projVelWorldSpace.y + (projAccel.y * timeStep);
		projVelWorldSpace.z = projVelWorldSpace.z + (projAccel.z * timeStep);

		// Update position from velocity
		projPosition.x = projPosition.x + (projVelWorldSpace.x * timeStep);
		projPosition.y = projPosition.y + (projVelWorldSpace.y * timeStep);
		projPosition.z = projPosition.z + (projVelWorldSpace.z * timeStep);

		// Draw a sphere at each of these locations...
		debugSphere->position = projPosition;
		debugSphere->setDiffuseColour(glm::vec3(0.8f, 0.8f, 0.8f));
		debugSphere->isVisible = true;
		debugSphere->scale = 0.075f;

		glm::mat4 matWorld = glm::mat4(1.0f);
		cSceneUtils::getInstance()->drawObject(debugSphere, matWorld, cShaderUtils::getInstance()->getIdFromFriendlyName(shaderProgramName));

		debugSphere->isVisible = false;
	}

}

