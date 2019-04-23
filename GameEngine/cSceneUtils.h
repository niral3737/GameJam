#ifndef _SCENE_UTILS_
#define _SCENE_UTILS_

#include <glad/glad.h>
#include <vector>
#include <glm/mat4x4.hpp>

#include "iMeshObject.h"
#include "cMeshObject.h"
#include "cAABB.h"
#include "cCommandGroup.h"

#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"
#include "cMazeMaker.h"
#include "cCharacter.h"
#include "cFBO.h"

class cSceneUtils
{
public:
	~cSceneUtils();
	static cSceneUtils* getInstance(void);

	void loadModelsIntoScene();
	static void initializeCamera();

	iMeshObject* findObjectByFriendlyName(std::string theNameToFind);
	void drawObject(iMeshObject* pCurrentMesh, glm::mat4x4 &matModel, GLuint shaderProgramID);
	void bindTextures(cMeshObject* pCurrentMesh, GLuint shaderProgramID);
	static glm::vec3 cameraEye;
	static glm::vec3 cameraAt;
	std::vector<cMeshObject*> vecObjectsToDraw;
	//cMeshObject* loadMeshInfoByFriendlyName( std::string friendlyName);
	void selectNextMeshObject(bool includeInvisibleObject);
	iMeshObject* selectedMeshObject;
	static bool loadFromSaveFile;
	
	void moveShip(float amount);

	std::vector<cMeshObject*> vecTrasparentObjects;
	cAABBHierarchy* terrainHierarchy;
	bool showAABBs;

	void drawSkyBox(glm::vec3 eye, GLuint program);
	void drawAABBs(GLuint program);

	cCommandGroup sceneCommandGroup;
	float dayMix;

	//skinned mesh
	//cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh;
	bool AssimpSM_to_VAO_Converter(cSimpleAssimpSkinnedMesh* pTheAssimpSM,
		unsigned int shaderProgramID);
	void LoadSkinnedMeshModel(GLuint shaderProgramID);
	float g_HACK_CurrentTime;
	std::vector<cCharacter*> characters;
	cCharacter* selectedCharacter;
	int selectedCharacterIndex;
	void selectNextCharacter();

	//maze
	const unsigned int MAZE_HEIGHT = 50;
	const unsigned int MAZE_WIDTH = 50;
	const float CUBE_LENGTH = 20.0f;
	cMazeMaker theMM;
	void generateMaze();
	void drawMaze(GLuint program);
	void placeDalek();
	unsigned int dalekLocation[2];

	void placePlayer();
	unsigned int playerLocation[2];
	glm::vec3 playerForward;
	void movePlayerZ(bool forward);
	void movePlayerX(bool right);
	float mDt;
	float timePassed;
	float walkAnimTime;
	glm::vec3 nextPlayerPosition;
	bool shouldUpdate;
	glm::vec3 nextCameraPosition;
	void moveCamera();
	glm::vec3 cameraDirToMove;

	cFBO* g_pFBOMain;
	bool blur;
	bool drawReticle;
	bool stencil;

	bool toon;
	void cartoonize();
private:
	static cSceneUtils* pSceneUtils;
	int selectedObjectIndex;
	cSceneUtils();
	void applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, glm::mat4& matModelInvTrans);
	bool HACK_bTextureUniformLocationsLoaded = false;
	GLint tex00_UniLoc = -1;
	GLint tex01_UniLoc = -1;
	GLint tex02_UniLoc = -1;
	GLint tex03_UniLoc = -1;
	GLint tex04_UniLoc = -1;
	GLint tex05_UniLoc = -1;
	GLint tex06_UniLoc = -1;
	GLint tex07_UniLoc = -1;

	// Texture sampler for off screen texture
	GLint texPass1OutputTexture_UniLoc = -1;

	GLint texBW_0_UniLoc = -1;
	GLint texBW_1_UniLoc = -1;
};

#endif // !_SCENE_UTILS_



