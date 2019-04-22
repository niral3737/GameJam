#ifndef _AABB_HG_
#define _AABB_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <map>
#include <vector>
#include "cModelDrawInfo.h"

class cAABB
{
public:
	cAABB();
	~cAABB();

	void setCentre(glm::vec3 centre);
	void setHalfSideLength(float halfSideLength);
	
	glm::vec3 getMinXYZ();
	glm::vec3 getMaxXYZ();
	glm::vec3 getCentre();
	float getSideLength();
	float getHalfSideLength();

	unsigned long long getId(void);
	static unsigned long long generateId(glm::vec3 centre);
	static unsigned long long generateId(glm::vec3 objectLocation, float halfSideLength);

	struct sAABBTriangle
	{
		glm::vec3 verts[3];
	};
	std::vector<sAABBTriangle> vecTriangles;

private:
	glm::vec3 centre;
	float halfSideLength;
};

class cAABBHierarchy
{
public:
	std::map<unsigned long long, cAABB*> mapAABBs;

	void loadTerrainAABB(cModelDrawInfo& terrainMeshInfo);
};

#endif // !_AABB_HG_


