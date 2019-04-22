#pragma once
#include "cParticleEmitter.h"
#include <map>
#include <string>
#include <glad/glad.h>

class cParticleManager
{
public:
	cParticleManager();
	~cParticleManager();

	void setUpParticleEmitters();
	void updateAndDrawParticles(double deltaTime, GLuint shaderProgramID,
		glm::vec3 cameraEye);
	void reset(std::string name);
	void loadUniformLocations(GLuint program);
	
private:
	std::map<std::string, cParticleEmitter*> mapParticleEmitters;
	GLint bIsParticleImposter_UniLoc;
	GLint ParticleImposterAlphaOverride_UniLoc;
	GLint ParticleImposterBlackThreshold_UniLoc;
};