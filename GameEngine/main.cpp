#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <iostream>
#include <fstream>
#include <sstream>

#include "cGLFWUtils.h"
#include "cShaderUtils.h"
#include "cJsonUtils.h"
#include "cVAOMeshUtils.h"
#include "cSceneUtils.h"
#include "iMeshObject.h"
#include "cUserIO.h"
#include "cPhysics.h"
#include "cLightsManager.h"
#include "cLightHelper.h"
#include "cLight.h"
//#include "iEquipment.h"
#include "cMediator.h"
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

#include "cCamera.h"
#include "cRandomHelper.h"
//#include "cPort.h"
#include "cEntityFactory.h"
#include "cCharacter.h"
#include "PhysicsGlobal.h"
#include <physics/interfaces/sRigidBodyDef.h>
#include "Flock.h"
#include "cRandThreaded.h"
#include "cBirdBufferManager.h"
#include "cBird.h"
#include "FormationBehaviour.h"
#include "cParticleManager.h"
#include "cAgentManager.h"

int main(int argc, char** argv)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();
	//std::cout << "Load from previously saved file? (Y/y)" << std::endl;
	char answer = 'y';
	//std::cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		cVAOMeshUtils::loadFromSaveFile = true;
		cLightsManager::loadFromSaveFile = true;
		cSceneUtils::loadFromSaveFile = true;
	}

	cSceneUtils::getInstance()->LoadSkinnedMeshModel(program);
	cVAOMeshUtils::getInstance()->loadModels(program);
	
	//Physics
	SetUpPhysics();
	nPhysics::sRigidBodyDef defChar1;
	defChar1.ShapeType = "Cylinder";
	defChar1.Mass = 1.0f;
	defChar1.Velocity = glm::vec3(0.0f);
	defChar1.Orientation = glm::vec3(0.0f);
	defChar1.HalfExtents = glm::vec3(7.8f, 10.0f, 7.8f);
	defChar1.Position = glm::vec3(0.0f);
	nPhysics::iShape* shapeChar1 = gPhysicsFactory->CreateCylinderShape(defChar1.HalfExtents);
	nPhysics::iRigidBody* rigidBodyChar1 = gPhysicsFactory->CreateRigidBody(defChar1, shapeChar1);


	nPhysics::sRigidBodyDef defPlane1;
	defPlane1.ShapeType = "Plane";
	defPlane1.Mass = 0.0f;
	defPlane1.Velocity = glm::vec3(0.0f);
	defPlane1.Orientation = glm::vec3(0.0f);
	defPlane1.Position = glm::vec3(0.0f);
	defPlane1.PlaneNormal = glm::vec3(0.0f, 1.0f, 0.0f);
	defPlane1.PlaneConstant = 1.0f;
	nPhysics::iShape* shapePlane1 = gPhysicsFactory->CreatePlaneShape(defPlane1.PlaneNormal, defPlane1.PlaneConstant);
	nPhysics::iRigidBody* rigidBodyPlane1 = gPhysicsFactory->CreateRigidBody(defPlane1, shapePlane1);

	nPhysics::sRigidBodyDef defPlane2;
	defPlane2.ShapeType = "Plane";
	defPlane2.Mass = 0.0f;
	defPlane2.Velocity = glm::vec3(0.0f);
	defPlane2.Orientation = glm::vec3(0.0f);
	defPlane2.Position = glm::vec3(30.0f, 150.0f, 0.0f);
	defPlane2.PlaneNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
	defPlane2.PlaneConstant = 1.0f;
	nPhysics::iShape* shapePlane2 = gPhysicsFactory->CreatePlaneShape(defPlane2.PlaneNormal, defPlane2.PlaneConstant);
	nPhysics::iRigidBody* rigidBodyPlane2 = gPhysicsFactory->CreateRigidBody(defPlane2, shapePlane2);
	//!Physics
	double lastTime = glfwGetTime();

	cLightsManager* lightsManager = cLightsManager::getInstance();

	cSceneUtils::initializeCamera();
	cSceneUtils::getInstance()->loadModelsIntoScene();

	lightsManager->loadAllLights(program);

	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	sceneUtils->initializeCamera();
	cCamera* camera = cCamera::getInstance();

	sceneUtils->g_pFBOMain = new cFBO();
	std::string FBOErrorString;
	// This is a 16x9 aspect ratio
	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int renderPassNumber = 1;
	// 1 = 1st pass (the actual scene)
	// 2 = 2nd pass (rendering what we drew to the output)
	GLint renderPassNumber_UniLoc = glGetUniformLocation(program, "renderPassNumber");
	std::cout << renderPassNumber_UniLoc << std::endl;

	glm::vec4 waterOffset = glm::vec4(0.0f);
	GLint waterOffset_location = glGetUniformLocation(program, "waterOffset");
	GLint dayMix_location = glGetUniformLocation(program, "dayMix");

	GLint useReticle_location = glGetUniformLocation(program, "useReticle");
	
	GLint blur_uniloc = glGetUniformLocation(program, "blur");
	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	if (sceneUtils->g_pFBOMain->init(fbWidth, fbHeight, FBOErrorString))
		//	if ( ::g_pFBOMain->init( 256, 256, FBOErrorString ) )
	{
		std::cout << "Framebuffer is good to go!" << std::endl;
	}
	else
	{
		std::cout << "Framebuffer is NOT complete" << std::endl;
	}

	cPhysics* physics = new cPhysics();

	//sceneUtils->generateMaze();
	//sceneUtils->placeDalek();

	cMeshObject* player = (cMeshObject*)sceneUtils->findObjectByFriendlyName("SM_Object");
	player->scale = 0.1f;
	//player->scale = 1.0f;
	player->currentAnimation = "Idle";
	player->setOrientationEulerAngles(0.0f, 0.0f, 0.0f, false);
	player->rigidBody = rigidBodyChar1;


	//plane->rigidBody = rigidBodyPlane2;


	nPhysics::iPhysicsWorld* physicsWorld = gPhysicsFactory->CreatePhysicsWorld();
	physicsWorld->AddBody(rigidBodyChar1);
	physicsWorld->AddBody(rigidBodyPlane1);
	//physicsWorld->AddBody(rigidBodyPlane2);
	physicsWorld->SetGravity(glm::vec3(0.0f, -9.8f, 0.0f));
	//sceneUtils->placePlayer();
	//camera->lookAt(player->position);

	//physics
	for (size_t i = 0; i < gRigidBodies.size(); i++)
	{
		physicsWorld->AddBody(gRigidBodies[i]);
	}
	//physics
	cCharacter* character1 = new cCharacter();
	character1->mesh = player;
	sceneUtils->characters.push_back(character1);

	cCharacter* character2 = new cCharacter();
	character2->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("GirlCharacter");
	character2->mesh->setOrientationEulerAngles(0.0f, 0.0f, 0.0f, false);
	character2->mesh->position = glm::vec3(-30.0f, 0.0f, 0.0f);
	//character2->mesh->isRefractive = true;

	sceneUtils->characters.push_back(character2);

	sceneUtils->selectNextCharacter();
	//Formation
	FormationBehaviour* behaviour = new FormationBehaviour();
	behaviour->centerPosition = glm::vec3(0.0f, 80.0f, 0.0f);
	behaviour->faceDirection = glm::vec3(-1.0f, 0.0f, -1.0f);

	const size_t NUM_BIRDS = 0;
	cMeshObject* eagle = (cMeshObject*)sceneUtils->findObjectByFriendlyName("eagle");
	eagle->isVisible = false;
	std::vector<cEntity*> formationEntities;
	const float SPAWN_RANGE = 200.0f;

	for (size_t i = 0; i < NUM_BIRDS; i++)
	{
		cEntity* entity = new cEntity();
		eagle->isVisible = true;
		entity->mesh = eagle->cloneObject();
		float x = cRandomHelper::generateRandomfloatInRange(-SPAWN_RANGE, SPAWN_RANGE);
		float z = cRandomHelper::generateRandomfloatInRange(-SPAWN_RANGE, SPAWN_RANGE);
		entity->mesh->position.x = x;
		entity->mesh->position.z = z;

		behaviour->addMember(entity->mesh->position);
		formationEntities.push_back(entity);
	}
	eagle->isVisible = false;
	behaviour->updateNewPositions();

	std::vector<cBird*> birds;
	cBirdBufferManager* bufferManager = new cBirdBufferManager();
	bufferManager->InitBuffers(NUM_BIRDS);
	for (size_t i = 0; i < NUM_BIRDS; i++)
	{
		cBird* bird = new cBird();
		bird->behaviour = behaviour;
		birds.push_back(bird);
	}

	for (size_t i = 0; i < birds.size(); i++)
	{
		bufferManager->UpdatePositionAtIndex(i, formationEntities[i]->mesh->position);
	}
	bufferManager->SwitchBuffers();
	for (size_t i = 0; i < birds.size(); i++)
	{
		bufferManager->UpdatePositionAtIndex(i, formationEntities[i]->mesh->position);
	}
	bufferManager->SwitchBuffers();
	for (size_t i = 0; i < birds.size(); i++)
	{
		bufferManager->UpdatePositionAtIndex(i, formationEntities[i]->mesh->position);
	}

	for (size_t i = 0; i < birds.size(); i++)
	{
		cRandThreaded* pRT = new cRandThreaded();
		birds[i]->setInitialPos(formationEntities[i]->mesh->position);
		birds[i]->CreateThreadAndStartUpdating(i, bufferManager, pRT);
	}

	bool TwoPass = false;
	//!Flocking
	//sceneUtils->drawReticle = true;
	//particle manager
	cParticleManager* particleManeger = new cParticleManager();
	particleManeger->loadUniformLocations(program);
	particleManeger->setUpParticleEmitters();
	particleManeger->reset("Smoke01");
	particleManeger->reset("Spark01");
	//!particle manager
	//behaviours
	eagle->isVisible = true;
	eagle->position = glm::vec3(100.0f, 10.0f, 100.0f);
	cAgentManager* agentManager = new cAgentManager();
	cMeshObject* NPC1 = (cMeshObject*)sceneUtils->findObjectByFriendlyName("NPCCharacter1");
	cMeshObject* NPC2 = (cMeshObject*)sceneUtils->findObjectByFriendlyName("NPCCharacter2");
	cMeshObject* NPC3 = (cMeshObject*)sceneUtils->findObjectByFriendlyName("NPCCharacter3");

	NPC1->position = glm::vec3(100.0f, 0.0f, 200.0f);
	NPC2->position = glm::vec3(-100.0f, 0.0f, 200.0f);

	agentManager->init();
	agentManager->seekAgent->updateTargetPos(character2->mesh->position);
	agentManager->seekAgent->updateTargetOri(character2->mesh->getOrientation());
	agentManager->seekAgent->initPosBuffer(NPC1->position);
	agentManager->seekAgent->initOriBuffer(NPC1->getOrientation());

	agentManager->fleeAgent->updateTargetPos(character2->mesh->position);
	agentManager->fleeAgent->updateTargetOri(character2->mesh->getOrientation());
	agentManager->fleeAgent->initPosBuffer(NPC2->position);
	agentManager->fleeAgent->initOriBuffer(NPC2->getOrientation());

	agentManager->wanderAgent->updateTargetPos(character2->mesh->position);
	agentManager->wanderAgent->updateTargetOri(character2->mesh->getOrientation());
	agentManager->wanderAgent->currentLocationToGo = glm::vec3(70.0f, 0.0f, 100.0f);
	agentManager->wanderAgent->initPosBuffer(NPC3->position);
	agentManager->wanderAgent->initOriBuffer(NPC3->getOrientation());

	agentManager->start();

	//!behaviours
	while (!glfwWindowShouldClose(window))
	{
		agentManager->seekAgent->updateTargetPos(character2->mesh->position);
		agentManager->seekAgent->updateTargetOri(character2->mesh->getOrientation());

		agentManager->fleeAgent->updateTargetPos(character2->mesh->position);
		agentManager->fleeAgent->updateTargetOri(character2->mesh->getOrientation());

		agentManager->wanderAgent->updateTargetPos(character2->mesh->position);
		agentManager->wanderAgent->updateTargetOri(character2->mesh->getOrientation());

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		double MAX_DELTA_TIME = 0.1;	// 100 ms
		int counter = 0;
		for (size_t i = 0; i < birds.size(); i++)
		{
			if (!birds[i]->bIsAlive)
			{
				counter++;
			}
		}

		if (counter == birds.size())
		{
			behaviour->nextFormationShape();
			behaviour->updateNewPositions();
			for (size_t i = 0; i < birds.size(); i++)
			{
				cRandThreaded* pRT = new cRandThreaded();
				birds[i]->setInitialPos(birds[i]->readPosition());
				birds[i]->CreateThreadAndStartUpdating(i, bufferManager, pRT);
			}
		}

		for (size_t i = 0; i < birds.size(); i++)
		{
			glm::vec3 pos = birds[i]->readPosition();
			formationEntities[i]->mesh->position = pos;
		}
		//behaviour
		NPC1->position = agentManager->seekAgent->readPosition();
		NPC1->setOrientation(agentManager->seekAgent->readOri());

		NPC2->position = agentManager->fleeAgent->readPosition();
		NPC2->setOrientation(agentManager->fleeAgent->readOri());

		NPC3->currentAnimation = "Unarmed-Walk";
		NPC3->position = agentManager->wanderAgent->readPosition();
		NPC3->setOrientation(agentManager->wanderAgent->readOri());

		if (glm::distance(NPC1->position, character2->mesh->position) <= 15.0f)
		{
			NPC1->currentAnimation = "Idle";
		}
		else
		{
			NPC1->currentAnimation = "Unarmed-Walk";
		}

		if (glm::distance(NPC2->position, character2->mesh->position) <= 15.0f)
		{
			NPC2->currentAnimation = "Idle";
		}
		else
		{
			NPC2->currentAnimation = "Unarmed-Walk";
		}

		agentManager->bufferManager->SwitchBuffers();
		//!//behaviour
		bufferManager->SwitchBuffers();
		
		if (TwoPass)
		{
			cShaderUtils::getInstance()->useShaderProgram("myShader");
			glBindFramebuffer(GL_FRAMEBUFFER, sceneUtils->g_pFBOMain->ID);

			//**********************************************************
			//     ___ _                _   _          ___ ___  ___  
			//    / __| |___ __ _ _ _  | |_| |_  ___  | __| _ )/ _ \ 
			//   | (__| / -_) _` | '_| |  _| ' \/ -_) | _|| _ \ (_) |
			//    \___|_\___\__,_|_|    \__|_||_\___| |_| |___/\___/ 
			//                                                       		
			// Clear the offscreen frame buffer
	//		glViewport( 0, 0, g_FBO_SizeInPixes, g_FBO_SizeInPixes );
	//		GLfloat	zero = 0.0f;
	//		GLfloat one = 1.0f;
	//		glClearBufferfv( GL_COLOR, 0, &zero );
	//		glClearBufferfv( GL_DEPTH, 0, &one );

			// Clear colour and depth buffers
			sceneUtils->g_pFBOMain->clearBuffers(true, true);
			glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass
		}
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		//glm::mat4x4 matProjection = glm::mat4(1.0f);
		//glm::mat4x4	matView = glm::mat4(1.0f);

		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4 matView = glm::mat4(1.0f);

		
		//ratio = 2500 / (float) 2500;
		//glViewport(0, 0, 2500, 2500);
		if (TwoPass)
		{
			ratio = width / (float)height;
			glViewport(0, 0, width, height);
		}
		else
		{
			ratio = width / (float) height;
			glViewport(0, 0, width, height);
		}

		//glfwGetFramebufferSize(window, &width, &height);
		//ratio = width / (float)height;
		//glViewport(0, 0, width, height);

		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
		//glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
		if (!TwoPass)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);	// Far clipping plane
		matView = glm::lookAt(cCamera::getInstance()->eye,	// Eye
			cCamera::getInstance()->getAtInWorldSpace(),		// At
			cCamera::getInstance()->getUpVector());// Up

		glUniform3f(eyeLocation_location, cCamera::getInstance()->eye.x, cCamera::getInstance()->eye.y, cCamera::getInstance()->eye.z);

		glUniform1f(dayMix_location, sceneUtils->dayMix);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		lightsManager->copyLightValuesToShader();
		lightsManager->drawAttenuationSpheres(program);

		lightsManager->dimLights(sceneUtils->dayMix);
		lightsManager->flickerLight("light3");
		lightsManager->flickerLight("light5");

		//STENCIL
		if (sceneUtils-> stencil)
		{
			glDisable(GL_STENCIL_TEST);
			glm::mat4 matRoom(1.0f);
			cMeshObject* pRoom = (cMeshObject*) sceneUtils->findObjectByFriendlyName("Room");
			pRoom->isVisible = true;
			sceneUtils->drawObject(pRoom, matRoom, program);
			pRoom->isVisible = false;
			// Keep the depth ON
		// Clear the stencil buffer
		// Set the stencil op to be ALWAYS and write some value
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			glStencilMask(0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			// DON'T write to the depth buffer (when drawing the doors)
			glDepthMask(GL_FALSE);

			// Draw the doorways
			glm::mat4 matDoors(1.0f);
			cMeshObject* pDoors = (cMeshObject*)sceneUtils->findObjectByFriendlyName("Doors");
			pDoors->isVisible = true;
			sceneUtils->drawObject(pDoors, matDoors, program);
			pDoors->isVisible = false;

			//		glDisable(GL_STENCIL_TEST);
					// now draw everything else:
			glDepthMask(GL_TRUE);		// Enable depth buffer write
			glStencilFunc(GL_EQUAL, 1, 0xFF);	//
//		glStencilFunc(GL_GEQUAL, 1, 0xFF);	//
		// DON'T update the stencil buffer, though
		// (keep doesn't touch the values that get WRITTEN to the stencil buffer)
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilMask(0x00);
		}

		//!STENCIL
		cSceneUtils::getInstance()->drawSkyBox(camera->eye, program);

		//glm::mat4x4 matModel = glm::mat4(1.0f);
		//eagle->position = glm::vec3(0.0f);
		//eagle->isVisible = true;
		//sceneUtils->drawObject(eagle, matModel, program);

		cSceneUtils::getInstance()->vecTrasparentObjects.clear();
		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)cSceneUtils::getInstance()->vecObjectsToDraw.size();
			objIndex++)
		{
			cMeshObject* pCurrentMesh = (cMeshObject*) cSceneUtils::getInstance()->vecObjectsToDraw[objIndex];

			//sorting logic
			if (pCurrentMesh->materialDiffuse.a < 1.0f)
			{
				if (cSceneUtils::getInstance()->vecTrasparentObjects.empty())
				{
					cSceneUtils::getInstance()->vecTrasparentObjects.push_back(pCurrentMesh);
					continue;
				}
				size_t numTransObj = cSceneUtils::getInstance()->vecTrasparentObjects.size();
				for (size_t i = 0; i < numTransObj; i++)
				{
					cMeshObject* obj = cSceneUtils::getInstance()->vecTrasparentObjects[i];
					float distanceObj = glm::distance(obj->position, camera->eye);
					float distanceCurrentMesh = glm::distance(pCurrentMesh->position, camera->eye);
					if (distanceObj < distanceCurrentMesh )
					{
						cSceneUtils::getInstance()->vecTrasparentObjects.insert(cSceneUtils::getInstance()->vecTrasparentObjects.begin() + i, pCurrentMesh);
						break;
					}
					else if (i == numTransObj - 1)
					{
						cSceneUtils::getInstance()->vecTrasparentObjects.push_back(pCurrentMesh);
						break;
					}
				}
				continue;
			}
			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			cSceneUtils::getInstance()->drawObject(pCurrentMesh, matModel, program);
		}//for ( unsigned int objIndex = 0; 

		//drawing trasparent objects
		for (size_t i = 0; i < cSceneUtils::getInstance()->vecTrasparentObjects.size(); i++)
		{
			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;
			cSceneUtils::getInstance()->drawObject(cSceneUtils::getInstance()->vecTrasparentObjects[i], matModel, program);
		}

		for (size_t i = 0; i < formationEntities.size(); i++)
		{
			glm::mat4x4 matModel = glm::mat4(1.0f);
			cSceneUtils::getInstance()->drawObject(formationEntities[i]->mesh, matModel, program);
		}
		particleManeger->updateAndDrawParticles(deltaTime, program, camera->eye);

		glDisable(GL_STENCIL_TEST);

		if (TwoPass)
		{
			// ****************************************
			// Now the entire scene has been drawn 
			// ****************************************

			// *****************************************
			// 2nd pass
			// *****************************************

			// 1. Set the Framebuffer output to the main framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer

			// Get the size of the actual (screen) frame buffer
			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float)height;
			glViewport(0, 0, width, height);

			//glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
			glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
			glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

			// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
			//     (NOT the offscreen buffer)
			// This clears the ACTUAL screen framebuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cMeshObject* p2SidedQuad = (cMeshObject*)sceneUtils->findObjectByFriendlyName("2SidedQuad");
			p2SidedQuad->isVisible = true;
			p2SidedQuad->b_HACK_UsesOffscreenFBO = true;
			p2SidedQuad->dontLight = true;
			p2SidedQuad->useVertexColor = false;
			//p2SidedQuad->materialDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			p2SidedQuad->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			//p2SidedQuad->bIsWireFrame = true;
			// Rotate it so it's "up and down"
			p2SidedQuad->setOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);
			p2SidedQuad->position.z = 1.0f;

			//ratio = 3840.0f / (float) 2160.0f;
			//p2SidedQuad->scale = ratio;

			// Tell the shader this is the 2nd pass...
			// This will run a very simple shader, which
			//  does NOT lighting, and only samples from a single texture
			//  (for now: soon there will be multiple textures)
			glUniform1f(renderPassNumber_UniLoc, 2.0f);	// Tell shader it's the 2nd pass
			if (sceneUtils->drawReticle)
			{
				glUniform1f(useReticle_location, (float)GL_TRUE);
			}
			else
			{
				glUniform1f(useReticle_location, (float)GL_FALSE);
			}

			if (sceneUtils->blur)
			{
				glUniform1f(blur_uniloc, (float)GL_TRUE);
			}
			else
			{
				glUniform1f(blur_uniloc, (float)GL_FALSE);
			}
			//glUniform1f(screenWidthUniLoc, (float) width);
			//glUniform1f(screenHeightUniLoc, (float) height);
			// Set the view transform so that the camera movement isn't impacted 

			glm::vec3 cameraFullScreenQuad = glm::vec3(0.0, 0.0, 0.0f);

			glUniform3f(eyeLocation_location, 0.0f, 0.0f, 0.0f);
			matView = glm::lookAt(glm::vec3(0.0, 0.0, 2.0f),	// Eye
				cameraFullScreenQuad,		// At
				glm::vec3(0.0f, 1.0f, 0.0f));// Up

			glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));

			// 4. Draw a single quad		
			glm::mat4 matModel = glm::mat4(1.0f);	// identity
			sceneUtils->drawObject(p2SidedQuad, matModel, program);
			p2SidedQuad->isVisible = false;
		}
		//// *****************************************
		//// 3rd pass
		//// *****************************************

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer

		//// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
		////     (NOT the offscreen buffer)
		//// This clears the ACTUAL screen framebuffer
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glUniform1f(renderPassNumber_UniLoc, 3.0f);	// Tell shader it's the 2nd pass

		//p2SidedQuad->isVisible = true;
		//// 4. Draw a single quad		
		//matModel = glm::mat4(1.0f);	// identity
		//sceneUtils->drawObject(p2SidedQuad, matModel, program);
		//// Make this invisible for the "regular" pass
		//p2SidedQuad->isVisible = false;

		//sceneUtils->drawMaze(program);

		//sceneUtils->drawAABBs(program);
		//std::cout << cAABB::generateId(ship->position, 10.0f) << std::endl;
		
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}
		sceneUtils->mDt = deltaTime;
		physicsWorld->Update(deltaTime);
		if (sceneUtils->walkAnimTime >= 0.75f && sceneUtils->shouldUpdate)
		{
			//player->position = sceneUtils->nextPlayerPosition;
			//camera->eye = sceneUtils->nextCameraPosition;
			sceneUtils->shouldUpdate = false;
			sceneUtils->selectedCharacter->isJumping = false;
			sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
		}
		//Flocking Integration
		//flock->CalculateSteering();
		for (size_t i = 0; i < formationEntities.size(); i++)
		{
			formationEntities[i]->mesh->velocity += (formationEntities[i]->mesh->acceleration * (float)deltaTime);
			formationEntities[i]->mesh->position += (formationEntities[i]->mesh->velocity * (float)deltaTime);
		}
		
		//!Flocking Integration
		/*if (sceneUtils->walkAnimTime <= 0.75f)
		{
			camera->eye += sceneUtils->cameraDirToMove * (0.01f / 0.75f);
		}*/

		//sceneUtils->sceneCommandGroup.Update(deltaTime);

		//std::cout << ship->position.x <<" " << ship->position.z << std::endl;
		//cModelDrawInfo modelDrawInfo;
		//modelDrawInfo.meshFileName = "terrain_xyz_n.ply";
		//cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

		physics->PhysicsUpdate(deltaTime, program);
		//physics->addProjectileAim(deltaTime);
		waterOffset.x += (0.1f * deltaTime);
		waterOffset.y += (0.017f * deltaTime);
		waterOffset.z -= (0.13f * deltaTime);
		waterOffset.w -= (0.013f * deltaTime);

		glUniform4f(waterOffset_location, waterOffset.x, waterOffset.y, waterOffset.z, waterOffset.w);

		lastTime = currentTime;

		/*cSceneUtils::getInstance()->drawEquipment(cueRack, program);
		for (size_t i = 0; i < ((cCueRack*)cueRack)->cues.size(); i++)
		{
			cSceneUtils::getInstance()->drawEquipment(((cCueRack*)cueRack)->cues[i], program);
		}*/

		glfwSwapBuffers(window);		// Shows what we drew

		glfwPollEvents();
		/*double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;*/

		cUserIO::processAsynKeys(window);
		cUserIO::processAsynMouse(window);
	}//while (!glfwWindowShouldClose(window))

	//soundManager->shutdownFmod();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

