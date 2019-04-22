#ifndef _C_CAMERA_HG_
#define _C_CAMERA_HG_

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>

class cCamera
{
public:
	~cCamera();
	static cCamera* getInstance();
	glm::vec3 eye;

	glm::vec3 getAtInWorldSpace();
	glm::vec3 getCameraDirection();
	glm::vec3 getUpVector();

	//mouse
	void setMouseXY(double newX, double newY);
	void setMouseWheelDelta(double deltaWheel);

	float getMouseX();
	float getMouseY();
	float getDeltaMouseX();
	float getDeltaMouseY();
	float getMouseWheel();

	float movementSpeed;

	void moveForwardZ(float amount);
	void moveLeftRightX(float amount);
	void moveUpDownY(float amount);

	void pitchUpDown(float angleDegrees);
	void yawLeftRight(float angleDegrees);
	void rollCWCCW(float angleDegrees);

	glm::quat getOrientation(void);
	void setOrientationEulerAngles(glm::vec3 newEulerAngles, bool bIsDegrees = false);
	void setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjustOrientationEulerAngles(glm::vec3 adjEulerAngle, bool bIsDegrees = false);
	void adjustOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjustOrientation(glm::quat adjOrient);
	
	void lookAt(glm::vec3 objPosToLookAt);

	glm::quat orientation;

private:
	cCamera();
	static cCamera* instance;

	glm::vec3 at;
	glm::vec3 up;
	glm::vec3 frontOfCamera;
	glm::vec3 upYVector;

	//mouse settings
	float lastMouseX;
	float lastMouseY;
	float lastMouseWheel;
	float mouseX;
	float mouseY;
	float mouseWheel;
	bool isMouseInitialStateSet;

	void updateAtFromOrientation();
	void updateUpFromOrientation();
};


#endif // !_C_CAMERA_HG_


