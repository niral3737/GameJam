#ifndef _LIGHTS_MANAGER_HG_
#define _LIGHTS_MANAGER_HG_

#include <glad/glad.h>
#include <string>
#include <vector>

#include "cLight.h"
#include "cLightHelper.h"

class cLightsManager
{
public:
	~cLightsManager();
	static bool loadFromSaveFile;

	std::vector<cLight*> vecLights;
	cLight* selectedLight;
	unsigned int selectedLightIndex;

	static cLightsManager* getInstance();

	void loadAllLights(GLuint program);
	void loadAllLightsFromSaveFile(GLuint program);

	void turnOnLight(int index);
	void changeLightType(std::string lightType, int index);
	cLight* getLightByFriendlyName(std::string friendlyName);

	// Do all the glGetUniformLocation() calls and store them...
	void loadUniformLocations(cLight* light, GLuint program, size_t index);

	// This would do the  nasty glUniformXf() calls
	// Start of every frame or scene
	void copyLightValuesToShader(void);
	void selectNextLight();
	
	void drawAttenuationSpheres(GLuint program);
	void dimLights(float dayMix);
	void flickerLight(std::string friendlyName);

	cLightHelper* pLightHelper;

private:
	cLightsManager();
	static cLightsManager* instance;
};

#endif // !_LIGHTS_MANAGER_HG_



