#include "cCamera.h"

cCamera* cCamera::instance = NULL;

cCamera::cCamera()
{
	this->eye = glm::vec3(0.0f);
	//this->at = glm::vec3(0.0f, 0.0f, 1.0f);

	this->frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);
	this->upYVector = glm::vec3(0.0f, 1.0f, 0.0f);

	this->movementSpeed = 1.0f; //this will change according to mouse wheel

	this->orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->updateAtFromOrientation();
	this->updateUpFromOrientation();

	this->setMouseXY(0.0, 0.0);
	this->mouseWheel = 25.0f;

	this->isMouseInitialStateSet = false;
}


cCamera::~cCamera()
{}

cCamera* cCamera::getInstance()
{
	if (instance == NULL)
	{
		instance = new cCamera();
	}
	return instance;
}

glm::vec3 cCamera::getAtInWorldSpace()
{
	return this->eye + this->at;
}

glm::vec3 cCamera::getCameraDirection()
{
	this->updateAtFromOrientation();

	return this->at;
}

glm::vec3 cCamera::getUpVector()
{
	return this->up;
}

void cCamera::setMouseXY(double newX, double newY)
{
	this->lastMouseX = this->mouseX;
	this->mouseX = (float) newX;

	this->lastMouseY = this->mouseY;
	this->mouseY = (float)newY;

	//can return value numbers
	this->isMouseInitialStateSet = true;
}

void cCamera::setMouseWheelDelta(double deltaWheel)
{
	this->lastMouseWheel = this->mouseWheel;
	this->mouseWheel += (float)deltaWheel;
}

float cCamera::getMouseX()
{
	return this->mouseX;
}

float cCamera::getMouseY()
{
	return this->mouseY;
}

float cCamera::getDeltaMouseX()
{
	if (!this->isMouseInitialStateSet)
	{
		return 0.0f;
	}
	return this->mouseX - this->lastMouseX;
}

float cCamera::getDeltaMouseY()
{
	if (!this->isMouseInitialStateSet)
	{
		return 0.0f;
	}
	return this->mouseY - this->lastMouseY;
}

float cCamera::getMouseWheel()
{
	return this->mouseWheel;
}

void cCamera::moveForwardZ(float amount)
{

	// We take the vector from the at to eye (i.e. direction)
// Add this forward velocity along this direction.

	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	// Make direction a "unit length"
	direction = glm::normalize(direction);

	// Generate a "forward" adjustment value 
	glm::vec3 amountToMove = direction * amount;

	// Add this to the eye
	this->eye += amountToMove;

	return;
}

void cCamera::moveLeftRightX(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cCamera::moveUpDownY(float amount)
{
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	this->eye += vecAmountToMove;

	return;
	
}

void cCamera::pitchUpDown(float angleDegrees)
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjustOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	this->updateAtFromOrientation();
	this->updateUpFromOrientation();

	return;
}

void cCamera::yawLeftRight(float angleDegrees)
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjustOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->updateAtFromOrientation();
	this->updateUpFromOrientation();

	return;
}

void cCamera::rollCWCCW(float angleDegrees)
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjustOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegrees), true);

	this->updateAtFromOrientation();
	this->updateUpFromOrientation();

	return;
}

glm::quat cCamera::getOrientation(void)
{
	return this->orientation;
}

void cCamera::setOrientationEulerAngles(glm::vec3 newEulerAngles, bool bIsDegrees)
{
	if (bIsDegrees)
	{
		newEulerAngles = glm::vec3(glm::radians(newEulerAngles.x),
			glm::radians(newEulerAngles.y),
			glm::radians(newEulerAngles.z));
	}

	this->orientation = glm::quat(glm::vec3(newEulerAngles.x, newEulerAngles.y, newEulerAngles.z));

	this->updateAtFromOrientation();
	this->updateUpFromOrientation();
}

void cCamera::setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees)
{
	this->setOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cCamera::adjustOrientationEulerAngles(glm::vec3 adjEulerAngle, bool bIsDegrees)
{
	if (bIsDegrees)
	{
		adjEulerAngle = glm::vec3(glm::radians(adjEulerAngle.x),
			glm::radians(adjEulerAngle.y),
			glm::radians(adjEulerAngle.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjEulerAngle);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	//this->orientation *= rotationAdjust;
	this->orientation *= rotationAdjust;
	this->orientation = this->orientation * rotationAdjust;

	this->updateAtFromOrientation();
	this->updateUpFromOrientation();
}

void cCamera::adjustOrientationEulerAngles(float x, float y, float z, bool bIsDegrees)
{
	this->adjustOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cCamera::adjustOrientation(glm::quat adjOrient)
{
	this->orientation *= adjOrient;
}

void cCamera::lookAt(glm::vec3 objPosToLookAt)
{
	glm::vec3 direction = glm::normalize(this->eye - objPosToLookAt);
	this->orientation = glm::quatLookAt(direction, glm::vec3(0.0f, 1.0f, 0.0f));

	updateAtFromOrientation();
	updateUpFromOrientation();
}

void cCamera::updateAtFromOrientation()
{
	// Have a rotation around the origin (eye)

	// vec4 new = mat4Transform * (vec4)Location

	// c'tor of mat4 can take a quaternion and generate 
	//	a rotation matrix from it... 
	glm::mat4 matRotation = glm::mat4(this->orientation);

	// Need to make these 4x4
	// NOTE: You could just keep these as 3x3 matrix values
	// (so you could keep them as vec3's....)

	glm::vec4 frontOfCamera = glm::vec4(this->frontOfCamera, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->at = glm::vec3(newAt);

	return;
}

void cCamera::updateUpFromOrientation()
{
	glm::mat4 matRotation = glm::mat4(this->orientation);

	glm::vec4 upVector = glm::vec4(this->upYVector, 1.0f);

	glm::vec4 newUp = matRotation * upVector;

	// Update the "At"
	this->up = glm::vec3(newUp);

	return;
}
