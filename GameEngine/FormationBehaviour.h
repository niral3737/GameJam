#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <glm/vec3.hpp>

class FormationBehaviour
{
public:
	enum eFormationShape
	{
		V,
		CIRCLE,
		//SQUARE,
		LINE,
		TWO_ROWS
	};
	FormationBehaviour();
	virtual ~FormationBehaviour();
	void addMember(glm::vec3 initPos);
	void setFormationShape(eFormationShape shape);
	void updateNewPositions();
	glm::vec3 centerPosition;
	glm::vec3 faceDirection;
	std::vector<glm::vec3> newPositions;
	std::vector<bool> reached;
	bool isReadyForNext;
	void nextFormationShape();
	int currentShape;
private:
	eFormationShape mShape;
	const float DISTANCE_OFFSET = 30.0f;
};

