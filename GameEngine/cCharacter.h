#pragma once
#include "cMeshObject.h"

class cCharacter
{
public:
	~cCharacter();
	cCharacter();
	cMeshObject* mesh;
	void move(float amount);
	void goInDir(float amount);
	void strafe(float amount);
	void jump();
	void action();
	bool isRunning;
	bool isJumping;
	bool isColliding;

private:
	glm::vec3 at;
};