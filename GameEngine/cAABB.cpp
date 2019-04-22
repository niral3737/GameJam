#include "cAABB.h"

#include "cSceneUtils.h"

cAABB::cAABB()
{}


cAABB::~cAABB()
{}

void cAABB::setCentre(glm::vec3 centre)
{
	this->centre = centre;
}

void cAABB::setHalfSideLength(float halfSideLength)
{
	this->halfSideLength = halfSideLength;
}

glm::vec3 cAABB::getMinXYZ()
{
	//assuming the AABB is aligned +X+Z plane with up as +Y
	return glm::vec3(getCentre().x - getHalfSideLength(),
					 getCentre().y - getHalfSideLength(),
					 getCentre().z - getHalfSideLength());
}
glm::vec3 cAABB::getMaxXYZ()
{
	return glm::vec3(getCentre().x + getHalfSideLength(),
		getCentre().y + getHalfSideLength(),
		getCentre().z + getHalfSideLength());
}

glm::vec3 cAABB::getCentre()
{
	return this->centre;
}

float cAABB::getSideLength()
{
	return this->halfSideLength * 2.0f;
}

float cAABB::getHalfSideLength()
{
	return this->halfSideLength;
}

unsigned long long cAABB::getId(void)
{
	return cAABB::generateId(getCentre(), getHalfSideLength());
}

unsigned long long cAABB::generateId(glm::vec3 centre)
{
	// 16,000,000,100,000,100,000 
	//    +xx,xxx +yy,yyy +zz,zzz  
	// If the value is NEGATIVE, then we will set the 6th 
	// digit to 1. If +ve then, it's 0

	// for example, xyz of -20, 460, -1280 would give:
	// 	// 100020 000460 101280	
	unsigned long long theABS_X = (unsigned long long(fabs(centre.x)));
	unsigned long long theABS_Y = (unsigned long long(fabs(centre.y)));
	unsigned long long theABS_Z = (unsigned long long(fabs(centre.z)));

	// Add the "sign" digit:
	// If +ve, then the sign is 0, eg: 193 would be:  000193   (000 193)
	// If -ve, then the sign is 1, eg: -193 would be: 100193   (100 193)
	if (centre.x < 0.0f) { theABS_X += 100000; }	// Sets 6th digit to 1
	if (centre.y < 0.0f) { theABS_Y += 100000; }	// Sets 6th digit to 1
	if (centre.z < 0.0f) { theABS_Z += 100000; }	// Sets 6th digit to 1

	unsigned long long theID =
		theABS_Z
		+ (theABS_Y * 1000000)				// Shift the y to the 7th digit
		+ (theABS_X * 1000000 * 1000000);	// Shift the x to the 13th

	return theID;
}

unsigned long long cAABB::generateId(glm::vec3 objectLocation, float halfSideLength)
{
	glm::vec3 centre;
	float sideLength = halfSideLength * 2.0f;
	
	centre.x = (floor(objectLocation.x / sideLength)) * sideLength + halfSideLength;
	centre.y = (floor(objectLocation.y / sideLength)) * sideLength + halfSideLength;
	centre.z = (floor(objectLocation.z / sideLength)) * sideLength + halfSideLength;
	
	return cAABB::generateId(centre);
}

void cAABBHierarchy::loadTerrainAABB(cModelDrawInfo& terrainMeshInfo)
{
	//// How big is our AABBs? Side length?
	//float halfSideLength = 30.0f;		// Play with this lenght
	//								// Smaller --> more AABBs, fewer triangles per AABB
	//								// Larger --> More triangles per AABB

	//float sideLength = halfSideLength * 2.0f;
	//cAABBHierarchy* g_pTheTerrain = cSceneUtils::getInstance()->terrainHierarchy;

	//for (unsigned int triIndex = 0; triIndex != terrainMeshInfo.numberOfTriangles; triIndex++)
	//{
	//	glm::ivec3 currentTri = terrainMeshInfo.pTriangles[triIndex];

	//	glm::vec3 currentVerts[3];
	//	currentVerts[0] = terrainMeshInfo.pMeshData->pVerticesFromFile[currentTri.x];
	//	currentVerts[1] = terrainMeshInfo.pVerticesFromFile[currentTri.y];
	//	currentVerts[2] = terrainMeshInfo.pVerticesFromFile[currentTri.z];

	//	// This is the structure we are eventually going to store in the AABB map...
	//	cAABB::sAABBTriangle curAABBTri;
	//	curAABBTri.verts[0].x = currentVerts[0].x;
	//	curAABBTri.verts[0].y = currentVerts[0].y;
	//	curAABBTri.verts[0].z = currentVerts[0].z;
	//	curAABBTri.verts[1].x = currentVerts[1].x;
	//	curAABBTri.verts[1].y = currentVerts[1].y;
	//	curAABBTri.verts[1].z = currentVerts[1].z;
	//	curAABBTri.verts[2].x = currentVerts[2].x;
	//	curAABBTri.verts[2].y = currentVerts[2].y;
	//	curAABBTri.verts[2].z = currentVerts[2].z;

	//	// Is the triangle "too big", and if so, split it (take centre and make 3 more)
	//	// (Pro Tip: "too big" is the SMALLEST side is greater than HALF the AABB length)
	//	// Use THOSE triangles as the test (and recursively do this if needed),
	//	// +++BUT+++ store the ORIGINAL triangle info NOT the subdivided one
	//	// 
	//	// For the student to complete... 
	//	// 

	//	//for now we're only considering one vertex 
	//	//TODO improve for all 3 vertices
	//	//for (unsigned int vertIndex = 0; vertIndex != 3; vertIndex++)
	//	//{
	//		// What AABB is "this" vertex in? 
	//	unsigned long long AABB_ID =
	//		cAABB::generateId(curAABBTri.verts[0],
	//			halfSideLength);

	//	// Do we have this AABB alredy? 
	//	std::map< unsigned long long/*ID AABB*/, cAABB* >::iterator itAABB
	//		= g_pTheTerrain->mapAABBs.find(AABB_ID);

	//	if (itAABB == g_pTheTerrain->mapAABBs.end())
	//	{
	//		// We DON'T have an AABB, yet
	//		cAABB* pAABB = new cAABB();
	//		// Determine the AABB location for this point
	//		// (like the generateID() method...)
	//		glm::vec3 vertexLocation = curAABBTri.verts[0];
	//		glm::vec3 centre;
	//		centre.x = (floor(vertexLocation.x / sideLength)) * sideLength + halfSideLength;
	//		centre.y = (floor(vertexLocation.y / sideLength)) * sideLength + halfSideLength;
	//		centre.z = (floor(vertexLocation.z / sideLength)) * sideLength + halfSideLength;

	//		pAABB->setCentre(centre);
	//		pAABB->setHalfSideLength(halfSideLength);
	//		// Note: this is the SAME as the AABB_ID...
	//		unsigned long long the_AABB_ID = pAABB->getId();

	//		g_pTheTerrain->mapAABBs[the_AABB_ID] = pAABB;

	//		// Then set the iterator to the AABB, by running find again
	//		itAABB = g_pTheTerrain->mapAABBs.find(the_AABB_ID);
	//	}//if( itAABB == ::g_pTheTerrain->m_mapAABBs.end() )

	//	// At this point, the itAABB ++IS++ pointing to an AABB
	//	// (either there WAS one already, or I just created on)

	//	itAABB->second->vecTriangles.push_back(curAABBTri);

	//	//}//for ( unsigned int vertIndex = 0;
	//}
}
