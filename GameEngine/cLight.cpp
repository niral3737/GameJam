#include "cLight.h"



cLight::cLight()
{}


cLight::~cLight()
{}

void cLight::setLinearAttenuation(float newLinAtten)
{
	this->atten.y = newLinAtten;
}

void cLight::setConstAttenuation(float newConstAtten)
{
	this->atten.x = newConstAtten;
}

void cLight::setQuadAttenuation(float newQuadAtten)
{
	this->atten.z = newQuadAtten;
}

void cLight::setDistanceCutOff(float distanceCutOff)
{
	this->atten.w = distanceCutOff;
}

void cLight::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = glm::vec4(diffuse, 1.0f);
}

void cLight::setPosition(glm::vec3 newPosition)
{
	this->position = glm::vec4(newPosition, 0.0f);
}

void cLight::setSpecularHighlight(glm::vec3 rgbColour, float power)
{
	this->specular = glm::vec4(rgbColour, power);
}

void cLight::setOn(bool on)
{
	this->param2.x = on ? 1.0f : 0.0f;
}

bool cLight::getOn()
{
	return this->param2.x > 0.0f;
}

void cLight::setLightType(std::string typeAsString)
{
	if (typeAsString == "Directional")
	{
		setLightType(DIRECTIONAL_LIGHT);
	}
	else if(typeAsString == "Spot")
	{
		setLightType(SPOT_LIGHT);
	}
	else
	{
		setLightType(POINT_LIGHT);
	}
}

void cLight::setLightType(eLightType lightType)
{
	switch (lightType)
	{
	case POINT_LIGHT:
		this->param1.x = 0.0f;		// Point
		break;
	case SPOT_LIGHT:
		this->param1.x = 1.0f;		// Spot
		break;
	case DIRECTIONAL_LIGHT:
		this->param1.x = 2.0f;		// Directional
		break;
	default:
		// Make point if we don't know
		// (shouldn't happen)
		this->param1.x = 0.0f;		// Point
		break;
	};

	return;
}

void cLight::setSpotConeAngles(float innerAngleDegrees, float outerAngleDegrees)
{
	//if ( outerAngleDegrees > innerAngleDegrees )
	//{	// Switch them
	//	float temp = innerAngleDegrees;
	//	innerAngleDegrees = outerAngleDegrees;
	//	outerAngleDegrees = temp;
	//}

	//	glm::vec4 param1;		// x = lightType, y = inner angle, z = outer angle, w = TBD
	this->param1.y = innerAngleDegrees;
	this->param1.z = outerAngleDegrees;

	return;
}

void cLight::setRelativeDirection(glm::vec3 relDirection)
{
	// To set the vec4 that's being passed
	this->direction = glm::vec4(relDirection, 1.0f);
	return;
}

void cLight::setRelativeDirectionByEulerAngles(glm::vec3 directionAngle)
{
	// Take the angles and make a quaternion out of them
	// Use a mat4x4 x vec4 tranformation (just like the shader or in physics)
	// Use the final xyz location to send to SetRelativeDirectionByLookAt()

	// Ta-Da!

	return;
}

void cLight::setRelativeDirectionByLookAt(glm::vec3 pointInWorldXYZ)
{
	// The vector from what I'm looking at to where I am, then normalize

	glm::vec3 lookVector = pointInWorldXYZ - glm::vec3(this->position);

	lookVector = glm::normalize(lookVector);

	this->setRelativeDirection(lookVector);

	return;
}
