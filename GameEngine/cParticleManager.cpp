#include "cParticleManager.h"
#include "cSceneUtils.h"

cParticleManager::cParticleManager()
{

}

cParticleManager::~cParticleManager()
{

}

void cParticleManager::setUpParticleEmitters()
{
	{
		cParticleEmitter* pPE_Smoke_01 = new cParticleEmitter();
		cParticleEmitter::sParticleCreationParams particleParams;
		particleParams.totalNumberOfParticles = 500;	// Max # of particles ALIVE at ONE TIME
		particleParams.minLifeTime = 5.0f;
		particleParams.maxLifeTime = 10.0f;
		// Will fade to fully transparent in the last 1 second of "life"
		particleParams.deathTransparencyFadeTimeSeconds = 2.0f;

		particleParams.position = glm::vec3(0.0f, 0.0f, 0.0f);
		particleParams.minInitialVelocity = glm::vec3(-0.5, 0.5, -0.5);
		particleParams.maxInitialVelocity = glm::vec3(0.5, 1.0, 0.5);
		particleParams.acceleration = glm::vec3(0.0f, 1.0f, 0.0f);
		particleParams.minNumberNewParticles = 2;
		particleParams.maxNumberNewParticles = 5;
		particleParams.minTimeBetweenParticleGenerationSeconds = 0.1f;
		particleParams.maxTimeBetweenParticleGenerationSeconds = 0.5f;
		particleParams.minInitialScale = 0.1f;
		particleParams.maxInitialScale = 0.15f;
		particleParams.minScaleChange = 0.0f;
		particleParams.maxScaleChange = 0.1f;	// This is per frame change

		// Degrees per frame
		particleParams.minOrientationChangeAngleEuler = glm::vec3(-0.25f, -0.25f, -0.25f);
		particleParams.maxOrientationChangeAngleEuler = glm::vec3(+0.25f, +0.25f, +0.25f);

		pPE_Smoke_01->Init(particleParams);

		mapParticleEmitters["Smoke01"] = pPE_Smoke_01;
	}
}

void cParticleManager::updateAndDrawParticles(double deltaTime, GLuint shaderProgramID,
	glm::vec3 cameraEye)
{
	// Black threshold is where the imposter will discard 
	// i.e. At or below this value, the imposter isn't draw. 
	// (range is from 0.0 to 1.0)
	glUniform1f(ParticleImposterBlackThreshold_UniLoc, 0.25f);

	// STARTOF: Star shaped smoke particle
	std::map<std::string /*name*/, cParticleEmitter* >::iterator itPE_Smoke01
		= mapParticleEmitters.find("Smoke01");

	if (itPE_Smoke01 != mapParticleEmitters.end())
	{

		cParticleEmitter* pPE_Smoke01 = itPE_Smoke01->second;

		// Update the particle emitter
		cMeshObject* pParticleMesh = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("SmokeObjectStar");
		glm::mat4 matParticleIndentity = glm::mat4(1.0f);
		glm::vec3 oldPosition = pParticleMesh->position;
		glm::quat oldOrientation = pParticleMesh->getOrientation();
		float oldScale = pParticleMesh->scale;

		pParticleMesh->setOrientationEulerAngles(0.0f, 0.0f, 0.0f);
		pParticleMesh->isVisible = true;


		// Set up the shader
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_TRUE);


		pPE_Smoke01->Update(deltaTime);

		std::vector<sParticle> vecParticlesToDraw;
		pPE_Smoke01->getAliveParticles(vecParticlesToDraw);

		pPE_Smoke01->sortParticlesBackToFront(vecParticlesToDraw, cameraEye);

		unsigned int numParticles = (unsigned int)vecParticlesToDraw.size();
		//			std::cout << "Drawing " << numParticles << " particles" << std::end;

		unsigned int count = 0;
		for (unsigned int index = 0; index != numParticles; index++)
		{
			if (vecParticlesToDraw[index].lifeRemaining > 0.0f)
			{
				// Draw it
				pParticleMesh->position = vecParticlesToDraw[index].position;
				pParticleMesh->scale = vecParticlesToDraw[index].scale;
				pParticleMesh->setOrientation(vecParticlesToDraw[index].qOrientation);

				// This is for the "death" transparency
				glUniform1f(ParticleImposterAlphaOverride_UniLoc, vecParticlesToDraw[index].transparency);

				cSceneUtils::getInstance()->drawObject(pParticleMesh, matParticleIndentity, shaderProgramID);
				count++;
			}
		}
		//			std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->isVisible = true;
		pParticleMesh->position = oldPosition;
		pParticleMesh->setOrientation(oldOrientation);
		pParticleMesh->scale = oldScale;
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_FALSE);
		//glUniform1f(ParticleImposterAlphaOverride_UniLoc, 1.0f);
		// ***************************************************************************
	}
	// ENDOF: Star shaped smoke particle
}

void cParticleManager::reset(std::string name)
{
	// STARTOF: Star shaped smoke particle
	std::map<std::string /*name*/, cParticleEmitter* >::iterator itPE_Smoke01
		= mapParticleEmitters.find(name);

	if (itPE_Smoke01 != mapParticleEmitters.end())
	{
		itPE_Smoke01->second->Reset(true);
	}
}

void cParticleManager::loadUniformLocations(GLuint program)
{
	bIsParticleImposter_UniLoc = glGetUniformLocation(program, "bIsParticleImposter");
	ParticleImposterAlphaOverride_UniLoc = glGetUniformLocation(program, "alphaOverride");
	ParticleImposterBlackThreshold_UniLoc = glGetUniformLocation(program, "ParticleImposterBlackThreshold");
}