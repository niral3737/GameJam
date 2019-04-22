#include "cUserIO.h"

#include "cSceneUtils.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "iMeshObject.h"
#include "cVAOMeshUtils.h"
#include "cMeshObject.h"
#include "cLight.h"
#include "cLightsManager.h"
#include "cJsonUtils.h"
#include "json.hpp"
#include "cCamera.h"
#include "cPhysics.h"
#include <glm/gtx/vector_angle.hpp>
#include "iEntity.h"
#include "cPlayer.h"

eSelectionMode cUserIO::selectionMode = eSelectionMode::MESH_SELECTION;
bool cUserIO::includeInvisibleObjects = false;

bool cUserIO::isMouseInsideWindow = false;

void cUserIO::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	cLightsManager* lightManager = cLightsManager::getInstance();
	cMeshObject* skinnedMesh = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("SM_Object");

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		sceneUtils->selectedCharacter->isRunning = false;
		sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
		sceneUtils->selectNextCharacter();
	}
	
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		sceneUtils->blur = !sceneUtils->blur;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		sceneUtils->drawReticle = !sceneUtils->drawReticle;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		sceneUtils->stencil = !sceneUtils->stencil;
	}

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		sceneUtils->selectedCharacter->isRunning = false;
		sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		sceneUtils->selectedCharacter->isRunning = false;
		sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		sceneUtils->selectedCharacter->isRunning = false;
		sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		sceneUtils->selectedCharacter->isRunning = false;
		sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
	}
	if ((key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
		|| (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_RELEASE))
	{
		sceneUtils->selectedCharacter->isRunning = false;
		sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
	}

	//const float CHAR_MOVE_JUMP_SPEED = 10.0f;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			sceneUtils->walkAnimTime = 0.0f;
			sceneUtils->shouldUpdate = true;
			sceneUtils->selectedCharacter->jump();
			//sceneUtils->selectedCharacter->goInDir(CHAR_MOVE_JUMP_SPEED);
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		sceneUtils->walkAnimTime = 0.0f;
		sceneUtils->shouldUpdate = true;
		sceneUtils->selectedCharacter->jump();
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		sceneUtils->selectedCharacter->action();
	}

	/*if (glfwGetKey(window, GLFW_KEY_UP) && action == GLFW_PRESS)
	{
		sceneUtils->movePlayerZ(true);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) && action == GLFW_PRESS)
	{
		sceneUtils->movePlayerZ(false);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) && action == GLFW_PRESS)
	{
		sceneUtils->movePlayerX(false);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) && action == GLFW_PRESS)
	{
		sceneUtils->movePlayerX(true);
	}*/

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		cMeshObject* islands= (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("islands");
		cCamera::getInstance()->lookAt(islands->position);
	}

	
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
	{
		skinnedMesh->currentAnimation = "Idle";
	}

	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
	{
		skinnedMesh->currentAnimation = "Unarmed-Walk";
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		selectionMode = eSelectionMode::MESH_SELECTION;
		std::cout << "Model selection mode" << std::endl;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		selectionMode = eSelectionMode::LIGHT_SELECTION;
		std::cout << "Light selection mode" << std::endl;
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		sceneUtils->showAABBs = !sceneUtils->showAABBs;
	}

	if (selectionMode == MESH_SELECTION && sceneUtils->selectedMeshObject)
	{
		cMeshObject* selectedObject = (cMeshObject*) sceneUtils->selectedMeshObject;
		if (glfwGetKey(window, GLFW_KEY_0) && action == GLFW_PRESS)
		{
			selectedObject->isWireFrame = !selectedObject->isWireFrame;
		}
		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) && action == GLFW_PRESS)
		{
			selectedObject->isVisible = !selectedObject->isVisible;
		}
		if (glfwGetKey(window, GLFW_KEY_9) && action == GLFW_PRESS)
		{
			selectedObject->dontLight = !selectedObject->dontLight;
		}
		if (glfwGetKey(window, GLFW_KEY_8) && action == GLFW_PRESS)
		{
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			selectedObject->materialDiffuse = glm::vec4(r, g, b, 1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_F1) && action == GLFW_PRESS)
		{
			selectedObject->materialDiffuse.a = selectedObject->materialDiffuse.a <= 0.0f ? 0.0f : selectedObject->materialDiffuse.a - 0.05;
		}
		if (glfwGetKey(window, GLFW_KEY_F2) && action == GLFW_PRESS)
		{
			selectedObject->materialDiffuse.a = selectedObject->materialDiffuse.a >= 1.0f ? 1.0f : selectedObject->materialDiffuse.a + 0.05;
		}
	}
	else if(selectionMode == LIGHT_SELECTION && lightManager->selectedLight){

		cLight* selectedLight = lightManager->selectedLight;

		if (glfwGetKey(window, GLFW_KEY_0) && action == GLFW_PRESS)
		{
			selectedLight->useDebugSphere = !selectedLight->useDebugSphere;
		}
		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) && action == GLFW_PRESS)
		{
			selectedLight->setOn(!selectedLight->getOn());
		}
		if (glfwGetKey(window, GLFW_KEY_9) && action == GLFW_PRESS)
		{
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			selectedLight->diffuse = glm::vec4(r, g, b, 1.0f);
		}
	}

	/*if (glfwGetKey(window, GLFW_KEY_EQUAL) && action == GLFW_PRESS)
	{
		cMeshObject* terrain = (cMeshObject*) sceneUtils->findObjectByFriendlyName("stadium");
		terrain->vecTextures[0].strength += (terrain->vecTextures[0].strength < 1.0f) ? 0.1f : 0.0f;
		terrain->vecTextures[1].strength = 1.0f - terrain->vecTextures[0].strength;
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) && action == GLFW_PRESS)
	{
		cMeshObject* terrain = (cMeshObject*)sceneUtils->findObjectByFriendlyName("stadium");
		terrain->vecTextures[0].strength -= (terrain->vecTextures[0].strength > 0.0f) ? 0.1f : 0.0f;
		terrain->vecTextures[1].strength = 1.0f - terrain->vecTextures[0].strength;
	}*/
	//}

	/*if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		cMeshObject* ball = (cMeshObject*) sceneUtils->findObjectByFriendlyName("cueBall");
		ball->isUpdatedByPhysics = true;
		ball->position = cCamera::getInstance()->eye;
		ball->velocity = cCamera::getInstance()->getCameraDirection() * cPhysics::initialProjectileVelocity;
		ball->acceleration = cPhysics::ACCEL_GRAVITY;

	}*/

	//save settings
	if (glfwGetKey(window, GLFW_KEY_ENTER))
	{
		mSaveSettings();
	}

	return;
}

void cUserIO::processAsynKeys(GLFWwindow* window)
{
	const float MOVE_SPEED = 0.4f;
	const float CAMERA_SPEED_SLOW = 0.03f;
	const float CAMERA_SPEED_FAST = 1.0f;

	const float CAMERA_TURN_SPEED = 0.1f;


	const float CHAR_MOVE_SPEED = 0.3f;
	const float CHAR_RUN_SPEED = 0.8f;
	const float CHAR_STRAFE_SPEED = 0.1f;

	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	cCamera* camera = cCamera::getInstance();

	float cameraMoveSpeed = camera->movementSpeed;
	// If no keys are down, move the camera
	if (mAreAllModifiersUp(window))
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			cPhysics::initialProjectileVelocity += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			cPhysics::initialProjectileVelocity -= 0.1f;
		}

		//animation
		//cMeshObject* skinnedMesh = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("SM_Object");
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			/*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				sceneUtils->walkAnimTime = 0.0f;
				sceneUtils->shouldUpdate = true;
				sceneUtils->selectedCharacter->jump();
				sceneUtils->selectedCharacter->goInDir(CHAR_MOVE_SPEED + 2.0f);
			}
			else
			{*/
				sceneUtils->selectedCharacter->move(CHAR_MOVE_SPEED);
			//}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->move(-CHAR_MOVE_SPEED);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->mesh->adjustOrientationEulerAngles(0.0f, -1.5f, 0.0f, true);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->mesh->adjustOrientationEulerAngles(0.0f, 1.5f, 0.0f, true);
		}

		/*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			sceneUtils->walkAnimTime = 0.0f;
			sceneUtils->shouldUpdate = true;
			sceneUtils->selectedCharacter->jump();
		}*/

		/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		{
			sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		{
			sceneUtils->selectedCharacter->mesh->currentAnimation = "Idle";
		}*/
		//!animation

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			//cSceneUtils::cameraEye.z += cameraSpeed;
			camera->moveForwardZ(+cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			//cSceneUtils::cameraEye.z -= cameraSpeed;
			camera->moveForwardZ(-cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			//cSceneUtils::cameraEye.x += cameraSpeed;
			camera->moveLeftRightX(-cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			//cSceneUtils::cameraEye.x -= cameraSpeed;
			camera->moveLeftRightX(+cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			//cSceneUtils::cameraEye.y += cameraSpeed;
			camera->moveUpDownY(-cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			//cSceneUtils::cameraEye.y -= cameraSpeed;
			camera->moveUpDownY(+cameraMoveSpeed);
		}

	}//if(AreAllModifiersUp(window)

	if (mIsShiftDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)	// "up"
		{
			sceneUtils->selectedCharacter->strafe(-CHAR_STRAFE_SPEED);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)	// "up"
		{
			sceneUtils->selectedCharacter->strafe(CHAR_STRAFE_SPEED);
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			camera->rollCWCCW(-CAMERA_TURN_SPEED);
			//			::g_pFlyCamera->MoveUpDown_Y( +cameraSpeed );
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			camera->rollCWCCW(+CAMERA_TURN_SPEED);
			//			::g_pFlyCamera->MoveUpDown_Y( -cameraSpeed );
		}
	}

	if (mIsAltDown(window))
	{
		cMeshObject* selectedMeshObject = (cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject;
		cLight* selectedLight = cLightsManager::getInstance()->selectedLight;
		if (selectionMode == MESH_SELECTION && selectedMeshObject)
		{
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				//selectedMeshObject->postRotation.x += glm::radians(1.0f);
				selectedMeshObject->adjustOrientationEulerAngles(1.0f, 0.0f, 0.0f, true);
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				//selectedMeshObject->postRotation.x -= glm::radians(1.0f);
				selectedMeshObject->adjustOrientationEulerAngles(-1.0f, 0.0f, 0.0f, true);
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				//selectedMeshObject->postRotation.z -= glm::radians(1.0f);
				selectedMeshObject->adjustOrientationEulerAngles(0.0f, 0.0f, -1.0f, true);
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				//selectedMeshObject->postRotation.z += glm::radians(1.0f);
				selectedMeshObject->adjustOrientationEulerAngles(0.0f, 0.0f, 1.0f, true);
			}
			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				//selectedMeshObject->postRotation.y += glm::radians(1.0f);
				selectedMeshObject->adjustOrientationEulerAngles(0.0f, 1.0f, 0.0f, true);
			}
			if (glfwGetKey(window, GLFW_KEY_E))
			{
				//selectedMeshObject->postRotation.y -= glm::radians(1.0f);
				selectedMeshObject->adjustOrientationEulerAngles(0.0f, -1.0f, 0.0f, true);
			}
		}
	}

	if (mIsCtrlDown(window))
	{
		cMeshObject* selectedMeshObject = (cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject;
		cLight* selectedLight = cLightsManager::getInstance()->selectedLight;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->isRunning = true;
			sceneUtils->selectedCharacter->move(CHAR_RUN_SPEED);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->isRunning = true;
			sceneUtils->selectedCharacter->move(-CHAR_RUN_SPEED);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->mesh->adjustOrientationEulerAngles(0.0f, -1.5f, 0.0f, true);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->mesh->adjustOrientationEulerAngles(0.0f, 1.5f, 0.0f, true);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			sceneUtils->selectedCharacter->jump();
		}
		
		if (selectionMode == MESH_SELECTION && (cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject)
		{
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				selectedMeshObject->position.z += MOVE_SPEED;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				selectedMeshObject->position.z -= MOVE_SPEED;
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				selectedMeshObject->position.x += MOVE_SPEED;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				selectedMeshObject->position.x -= MOVE_SPEED;
			}
			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				selectedMeshObject->position.y += MOVE_SPEED;
			}
			if (glfwGetKey(window, GLFW_KEY_E))
			{
				selectedMeshObject->position.y -= MOVE_SPEED;
			}
			if (glfwGetKey(window, GLFW_KEY_EQUAL))
			{
				selectedMeshObject->setSpecularPower(selectedMeshObject->getSpecularPower() + 1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_MINUS))
			{
				if (selectedMeshObject->getSpecularPower() <= 1.0f)
				{
					return;
				}
				selectedMeshObject->setSpecularPower(selectedMeshObject->getSpecularPower() - 1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_PERIOD))
			{
				selectedMeshObject->scale = selectedMeshObject->scale * 1.01f;
			}
			if (glfwGetKey(window, GLFW_KEY_COMMA))
			{
				selectedMeshObject->scale = selectedMeshObject->scale * 0.99f;
			}
		}
		else if (selectionMode == LIGHT_SELECTION && selectedLight)
		{
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				selectedLight->position.z += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				selectedLight->position.z -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				selectedLight->position.x += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				selectedLight->position.x -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				selectedLight->position.y += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_E))
			{
				selectedLight->position.y -= 0.5f;
			}

			//atten linear
			if (glfwGetKey(window, GLFW_KEY_EQUAL))
			{
				selectedLight->atten.y += 0.001f;
			}
			if (glfwGetKey(window, GLFW_KEY_MINUS))
			{
				selectedLight->atten.y -= 0.001f;
			}

			//atten quad
			if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET))
			{
				selectedLight->atten.z -= 0.00001f;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET))
			{
				selectedLight->atten.z += 0.00001f;
			}
		}
	}
}

bool cUserIO::mIsShiftDown(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool cUserIO::mIsCtrlDown(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool cUserIO::mIsAltDown(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool cUserIO::mAreAllModifiersUp(GLFWwindow * window)
{
	if (mIsShiftDown(window)) { return false; }
	if (mIsCtrlDown(window)) { return false; }
	if (mIsAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void cUserIO::processAsynMouse(GLFWwindow * window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	cCamera* camera = cCamera::getInstance();

	camera->setMouseXY(x, y);

	const float MOUSE_SENSITIVITY = 0.1f;

	//std::cout << camera->getMouseX() << ", " << camera->getMouseY() << std::endl;
		// Mouse left (primary?) button pressed? 
		// AND the mouse is inside the window...
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		&& cUserIO::isMouseInsideWindow)
	{
		// Mouse button is down so turn the camera
		camera->yawLeftRight(-camera->getDeltaMouseX() * MOUSE_SENSITIVITY);

		camera->pitchUpDown(camera->getDeltaMouseY() * MOUSE_SENSITIVITY);

	}

	// Adjust the mouse speed
	if (cUserIO::isMouseInsideWindow)
	{
		const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

		// Adjust the movement speed based on the wheel position
		camera->movementSpeed = camera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY;
		if (camera->movementSpeed <= 0.0f)
		{
			camera->movementSpeed = 0.0f;
		}
	}

	return;
}

void cUserIO::cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		cUserIO::isMouseInsideWindow = true;
		//std::cout << "Mouse moved indide window" << std::endl;
	}
	else
	{
		cUserIO::isMouseInsideWindow = false;
		//std::cout << "Mouse moved outside window" << std::endl;
	}
	return;
}//cursor_enter_callback(...

// Mouse (cursor) callback
void cUserIO::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	return;
}


void cUserIO::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	return;
}

void cUserIO::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	cCamera::getInstance()->setMouseWheelDelta(yoffset);

	//	std::cout << "Mouse wheel: " << yoffset << std::endl;

	return;
}

void cUserIO::mSaveSettings()
{
	cLightsManager* lightsManager = cLightsManager::getInstance();
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();

	std::ofstream ofs;
	ofs.open("savefile.json", std::ofstream::out | std::ofstream::trunc);
	nlohmann::json json = cJsonUtils::getJsonInstance();

	//save camera
	json["cameraEye"]["x"] = cCamera::getInstance()->eye.x;
	json["cameraEye"]["y"] = cCamera::getInstance()->eye.y;
	json["cameraEye"]["z"] = cCamera::getInstance()->eye.z;

	json["cameraAt"]["x"] = cCamera::getInstance()->getAtInWorldSpace().x;
	json["cameraAt"]["y"] = cCamera::getInstance()->getAtInWorldSpace().y;
	json["cameraAt"]["z"] = cCamera::getInstance()->getAtInWorldSpace().z;

	//saving the lights
	size_t numLights = lightsManager->vecLights.size();
	for (unsigned int index = 0; index != numLights; index++)
	{
		cLight* light = lightsManager->vecLights[index];

		json["lights"][index]["friendlyName"] = light->friendlyName;
		json["lights"][index]["useDebugSphere"] = light->useDebugSphere;

		json["lights"][index]["position"]["x"] = light->position.x;
		json["lights"][index]["position"]["y"] = light->position.y;
		json["lights"][index]["position"]["z"] = light->position.z;
		json["lights"][index]["position"]["w"] = light->position.w;

		json["lights"][index]["attenuation"]["const"] = light->atten.x;
		json["lights"][index]["attenuation"]["linear"] = light->atten.y;
		json["lights"][index]["attenuation"]["quad"] = light->atten.z;
		json["lights"][index]["attenuation"]["distanceCutOff"] = light->atten.w;

		json["lights"][index]["diffuse"]["r"] = light->diffuse.r;
		json["lights"][index]["diffuse"]["g"] = light->diffuse.g;
		json["lights"][index]["diffuse"]["b"] = light->diffuse.b;
		json["lights"][index]["diffuse"]["a"] = light->diffuse.a;

		json["lights"][index]["specular"]["r"] = light->specular.r;
		json["lights"][index]["specular"]["g"] = light->specular.g;
		json["lights"][index]["specular"]["b"] = light->specular.b;
		json["lights"][index]["specular"]["a"] = light->specular.a;

		json["lights"][index]["direction"]["x"] = light->direction.x;
		json["lights"][index]["direction"]["y"] = light->direction.y;
		json["lights"][index]["direction"]["z"] = light->direction.z;
		json["lights"][index]["direction"]["w"] = light->direction.w;

		json["lights"][index]["param1"]["lightType"] = light->param1.x;
		json["lights"][index]["param1"]["innerAngle"] = light->param1.y;
		json["lights"][index]["param1"]["outerAngle"] = light->param1.z;
		json["lights"][index]["param1"]["w"] = light->param1.w;

		json["lights"][index]["param2"]["on"] = light->param2.x;
		json["lights"][index]["param2"]["y"] = light->param2.y;
		json["lights"][index]["param2"]["z"] = light->param2.z;
		json["lights"][index]["param2"]["w"] = light->param2.w;
	}

	//saving meshobjects
	size_t numObjects = sceneUtils->vecObjectsToDraw.size();
	for (unsigned int index = 0; index != numObjects; index++)
	{
		cMeshObject* object = (cMeshObject*) sceneUtils->vecObjectsToDraw[index];
		json["meshes"][index]["meshName"] = object->meshName;
		json["meshes"][index]["friendlyName"] = object->friendlyName;
		json["meshes"][index]["isWireFrame"] = object->isWireFrame;
		json["meshes"][index]["isVisible"] = object->isVisible;
		json["meshes"][index]["useVertexColor"] = object->useVertexColor;
		json["meshes"][index]["dontLight"] = object->dontLight;

		json["meshes"][index]["position"]["x"] = object->position.x;
		json["meshes"][index]["position"]["y"] = object->position.y;
		json["meshes"][index]["position"]["z"] = object->position.z;

		glm::vec3 eularAngles = glm::eulerAngles(object->getOrientation());
		json["meshes"][index]["postRotation"]["x"] = eularAngles.x;
		json["meshes"][index]["postRotation"]["y"] = eularAngles.y;
		json["meshes"][index]["postRotation"]["z"] = eularAngles.z;

		json["meshes"][index]["materialDiffuse"]["r"] = object->materialDiffuse.r;
		json["meshes"][index]["materialDiffuse"]["g"] = object->materialDiffuse.g;
		json["meshes"][index]["materialDiffuse"]["b"] = object->materialDiffuse.b;
		json["meshes"][index]["materialDiffuse"]["a"] = object->materialDiffuse.a;

		json["meshes"][index]["materialSpecular"]["r"] = object->materialSpecular.r;
		json["meshes"][index]["materialSpecular"]["g"] = object->materialSpecular.g;
		json["meshes"][index]["materialSpecular"]["b"] = object->materialSpecular.b;
		json["meshes"][index]["materialSpecular"]["power"] = object->materialSpecular.a;

		json["meshes"][index]["scale"] = object->scale;

		json["meshes"][index]["isUpdatedByPhysics"] = object->isUpdatedByPhysics;

		json["meshes"][index]["velocity"]["x"] = object->velocity.x;
		json["meshes"][index]["velocity"]["y"] = object->velocity.y;
		json["meshes"][index]["velocity"]["z"] = object->velocity.z;

		json["meshes"][index]["acceleration"]["x"] = object->acceleration.x;
		json["meshes"][index]["acceleration"]["y"] = object->acceleration.y;
		json["meshes"][index]["acceleration"]["z"] = object->acceleration.z;

		for (size_t j = 0; j < object->vecTextures.size(); j++)
		{
			json["meshes"][index]["textures"][j]["name"] = object->vecTextures[j].name;
			json["meshes"][index]["textures"][j]["strength"] = object->vecTextures[j].strength;
		}
	}

	ofs << std::setw(4) << json << std::endl;
	ofs.close();
	std::cout << "settings saved" << std::endl;
}
