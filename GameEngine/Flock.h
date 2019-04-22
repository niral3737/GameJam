#ifndef FLOACK_HG
#define FLOACK_HG

#include "cEntity.h"
#include <vector>
#include <glm/glm.hpp>

class Flock
{
public:
	Flock(void);
	Flock(float cohesionWeight, float separationWeight, float alignmentWeight);
	virtual ~Flock(void);

	void AddFlockMember(cEntity* entity);
	void RemoveFlockMember(cEntity* entity);

	void CalculateSteering(void);
	void addCohesion(float delCohesion);
	void addAllignment(float delAllignment);
	void addSeparation(float delSeparation);

	glm::vec3 centerPoint;
private:
	struct FlockMemberInfo
	{
		FlockMemberInfo(cEntity* entity);
		~FlockMemberInfo(void);

		cEntity* entity;
	};
	glm::vec3 calculateAlignment(FlockMemberInfo* member);
	glm::vec3 calculateCohesion(FlockMemberInfo* member);
	glm::vec3 calculateSeparation(FlockMemberInfo* member);

	std::vector<FlockMemberInfo*> mFlockMembers;

	float cohesionWeight;
	float alignmentWeight;
	float separationWeight;

	float radius;
};


#endif // !FLOACK_HG
