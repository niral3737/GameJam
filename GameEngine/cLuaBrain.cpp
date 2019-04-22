#include "cLuaBrain.h"

#include <glm/glm.hpp>
#include <iostream>
#include "cMoveToCommand.h"
#include "json.hpp"
#include "cSceneUtils.h"
#include "cFollowObjectCommand.h"
#include "cOrientToCommand.h"
#include "cAnimationCurveCommand.h"
#include "cMoveToLookAt.h"
#include "cLocationTriggerMoveOrient.h"

cMeshObject* cLuaBrain::pMyGameObject = NULL;
bool cLuaBrain::inParallel = false;

int KillAllHumans(lua_State *L)
{
	// There are 3 things on the stack: "Justin", 47, 3.14159 
	const char* name = lua_tostring(L, 1);	// get "Justin"		
	int age = lua_tonumber(L, 2);			// "Cat"
	float pi = lua_tonumber(L, 3);			// 3.14159

	std::cout << "KillAllHumans(): "
		<< name << ", " << age << ", " << pi << std::endl;
	return 0;
}

cLuaBrain::cLuaBrain()
{
	this->m_p_vecGOs = nullptr;

	// Create new Lua state.
	// NOTE: this is common to ALL script in this case
	this->m_pLuaState = luaL_newstate();

	luaL_openlibs(this->m_pLuaState);					/* Lua 5.3.3 */

//	lua_pushcfunction( this->m_pLuaState, l_KillAllHumans );
//	lua_setglobal( this->m_pLuaState, "WhatBenderTheRobotSays" );

	lua_pushcfunction( this->m_pLuaState, KillAllHumans);
	lua_setglobal( this->m_pLuaState, "JustinTrudeauIsOurPM" );

	// 	static int l_UpdateObject( lua_State *L );		// c function
	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_UpdateObject );
	lua_setglobal( this->m_pLuaState, "setObjectState" );

	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_GetObjectState );
	lua_setglobal( this->m_pLuaState, "getObjectState" );

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_MoveWithTime);
	lua_setglobal(this->m_pLuaState, "moveWithTime");

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_OrientWithTime);
	lua_setglobal(this->m_pLuaState, "orientWithTime");

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_parallel);
	lua_setglobal(this->m_pLuaState, "parallel");

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_FollowObject);
	lua_setglobal(this->m_pLuaState, "followObject");

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_FollowCurve);
	lua_setglobal(this->m_pLuaState, "followCurve");

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_MoveToLookAt);
	lua_setglobal(this->m_pLuaState, "moveToLookAt");

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_LocationTriggerMoveOrient);
	lua_setglobal(this->m_pLuaState, "locationTriggerMoveOrient");

	return;
}

cLuaBrain::~cLuaBrain()
{
	lua_close(this->m_pLuaState);
	return;
}


// Saves (and overwrites) any script
// scriptName is just so we can delete them later
void cLuaBrain::LoadScript( std::string scriptName, 
					        std::string scriptSource )
{
	this->m_mapScripts[scriptName] = scriptSource;
	return;
}


void cLuaBrain::DeleteScript( std::string scriptName )
{
	// TODO: delete this scritp
	return;
}

// Passes a pointer to the game object vector
void cLuaBrain::SetObjectVector( std::vector< cMeshObject* >* p_vecGOs )
{
	this->m_p_vecGOs = p_vecGOs;
	return;
}

void cLuaBrain::RunThis(std::string theLuaScript)
{
	int error = luaL_loadstring( this->m_pLuaState,
								theLuaScript.c_str() );

	if (error != 0 /*no error*/)
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
					  0,	/* nargs: number of arguments pushed onto the lua stack */
					  0,	/* nresults: number of results that should be on stack at end*/
					  0);	/* errfunc: location, in stack, of error function.
							if 0, results are on top of stack. */
	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(this->m_pLuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

	}

	return;
}




// Call all the active scripts that are loaded
void cLuaBrain::Update(float deltaTime)
{
//	std::cout << "cLuaBrain::Update() called" << std::endl;
	for( std::map< std::string /*name*/, std::string /*source*/>::iterator itScript = 
		 this->m_mapScripts.begin(); itScript != this->m_mapScripts.end(); itScript++ )
	{

		// Pass the script into Lua and update
//		int error = luaL_loadbuffer(L, buffer, strlen(buffer), "line");

		std::string curLuaScript = itScript->second;

		int error = luaL_loadstring( this->m_pLuaState, 
									 curLuaScript.c_str() );

		if ( error != 0 /*no error*/)	
		{
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			continue;
		}

		// execute funtion in "protected mode", where problems are 
		//  caught and placed on the stack for investigation
		error = lua_pcall(this->m_pLuaState,	/* lua state */
						  0,	/* nargs: number of arguments pushed onto the lua stack */
						  0,	/* nresults: number of results that should be on stack at end*/
						  0);	/* errfunc: location, in stack, of error function. 
								   if 0, results are on top of stack. */
		if ( error != 0 /*no error*/)	
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append( lua_tostring(this->m_pLuaState, -1) );

			// Make error message a little more clear
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

			continue;
		}
	}

	// TODO: Lots of Lua stuff here...
	return;
}

// Called by Lua
// Passes object ID, new velocity, etc.
// Returns valid (true or false)
// Passes: 
// - position (xyz)
// - velocity (xyz)
// called "setObjectState" in lua
/*static*/ int cLuaBrain::l_UpdateObject( lua_State *L )
{
	int objectID = lua_tonumber(L, 1);	/* get argument */
	
	// Exist? 
	cMeshObject* pGO = cLuaBrain::m_findObjectByID(objectID);

	if ( pGO == nullptr )
	{	// No, it's invalid
		lua_pushboolean( L,  false );
		// I pushed 1 thing on stack, so return 1;
		return 1;	
	}

	// Object ID is valid
	// Get the values that lua pushed and update object
	pGO->position.x = lua_tonumber(L, 2);	/* get argument */
	pGO->position.y = lua_tonumber(L, 3);	/* get argument */
	pGO->position.z = lua_tonumber(L, 4);	/* get argument */
	pGO->velocity.x = lua_tonumber(L, 5);	/* get argument */
	pGO->velocity.y = lua_tonumber(L, 6);	/* get argument */
	pGO->velocity.z = lua_tonumber(L, 7);	/* get argument */

	lua_pushboolean( L, true );	// index is OK
	
	return 1;		// I'm returning ONE thing

}

// Passes object ID
// Returns valid (true or false)
// - position (xyz)
// - velocity (xyz)
// called "getObjectState" in lua
/*static*/ int cLuaBrain::l_GetObjectState( lua_State *L )
{
	int objectID = lua_tonumber(L, 1);	/* get argument */
	
	// Exist? 
	cMeshObject* pGO = cLuaBrain::m_findObjectByID(objectID);

	if ( pGO == nullptr )
	{	// No, it's invalid
		lua_pushboolean( L,  false );
		// I pushed 1 thing on stack, so return 1;
		return 1;	
	}

	// Object ID is valid
	lua_pushboolean( L, true );	// index is OK
	lua_pushnumber( L, pGO->position.x );
	lua_pushnumber( L, pGO->position.y );
	lua_pushnumber( L, pGO->position.z );
	lua_pushnumber( L, pGO->velocity.x );		
	lua_pushnumber( L, pGO->velocity.y );		
	lua_pushnumber( L, pGO->velocity.z );		
	
	return 7;		// There were 7 things on the stack
}

int cLuaBrain::l_parallel(lua_State * L)
{
	inParallel = lua_toboolean(L, 1);

	cCommandGroup* parallelCG = new cCommandGroup();
	parallelCG->friendlyName = "parallelCG";

	cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(parallelCG);

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

int cLuaBrain::l_LocationTriggerMoveOrient(lua_State *L)
{
	std::string objToMoveName = lua_tostring(L, 1);
	cMeshObject* objToMove = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName(objToMoveName);

	std::string objTriggeringName = lua_tostring(L, 2);
	cMeshObject* objTriggering = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName(objTriggeringName);

	glm::vec3 to = glm::vec3(0.0f);
	glm::vec3 toAngles = glm::vec3(0.0f);
	float triggerDistance = lua_tonumber(L, 3);
	to.x = lua_tonumber(L, 4);
	to.y = lua_tonumber(L, 5);
	to.z = lua_tonumber(L, 6);
	toAngles.x = lua_tonumber(L, 7);
	toAngles.y = lua_tonumber(L, 8);
	toAngles.z = lua_tonumber(L, 9);
	double time = lua_tonumber(L, 10);

	cLocationTriggerMoveOrient* command = new cLocationTriggerMoveOrient();

	nlohmann::json values;

	values["triggerDistance"] = triggerDistance;

	values["to"]["x"] = to.x;
	values["to"]["y"] = to.y;
	values["to"]["z"] = to.z;

	values["toAngles"]["x"] = toAngles.x;
	values["toAngles"]["y"] = toAngles.y;
	values["toAngles"]["z"] = toAngles.z;

	//time in seconds
	values["time"] = time;

	//time in seconds
	command->setObject(objToMove);
	command->setTriggeringObject(objTriggering);
	command->initialize(values);

	if (inParallel)
	{
		cCommandGroup* parallelCG = cSceneUtils::getInstance()->sceneCommandGroup.findCommandGroupByFrinedlyName("parallelCG");
		parallelCG->vecCommands.push_back(command);
		//cSceneUtils::getInstance()->sceneCommandGroup.vecCommands.push_back(command);
	}
	else
	{
		cCommandGroup* cg = new cCommandGroup();
		cg->vecCommands.push_back(command);
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(cg);
	}

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

int cLuaBrain::l_MoveToLookAt(lua_State *L)
{
	std::string objName = lua_tostring(L, 1);
	cMeshObject* pGO = NULL;
	if (objName != "camera")
	{
		pGO = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName(objName);

		if (pGO == nullptr)
		{	// No, it's invalid
			lua_pushboolean(L, false);
			// I pushed 1 thing on stack, so return 1;
			return 1;
		}
	}

	// Object ID is valid
	// Get the values that lua pushed and update object
	glm::vec3 from = glm::vec3(0.0f);
	glm::vec3 to = glm::vec3(0.0f);
	glm::vec3 lookAt = glm::vec3(0.0f);
	from.x = lua_tonumber(L, 2);
	from.y = lua_tonumber(L, 3);
	from.z = lua_tonumber(L, 4);
	to.x = lua_tonumber(L, 5);
	to.y = lua_tonumber(L, 6);
	to.z = lua_tonumber(L, 7);
	lookAt.x = lua_tonumber(L, 8);
	lookAt.y = lua_tonumber(L, 9);
	lookAt.z = lua_tonumber(L, 10);
	double time = lua_tonumber(L, 11);
	double easeOutTime = lua_tonumber(L, 12);

	cMoveToLookAt* command = new cMoveToLookAt();

	nlohmann::json values;

	values["from"]["x"] = from.x;
	values["from"]["y"] = from.y;
	values["from"]["z"] = from.z;

	values["to"]["x"] = to.x;
	values["to"]["y"] = to.y;
	values["to"]["z"] = to.z;

	values["lookAt"]["x"] = lookAt.x;
	values["lookAt"]["y"] = lookAt.y;
	values["lookAt"]["z"] = lookAt.z;

	//time in seconds
	values["time"] = time;
	values["easeOutTime"] = easeOutTime;

	//time in seconds
	if (objName != "camera")
	{
		command->setObject(pGO);
		values["isCamera"] = false;
	}
	else
	{
		values["isCamera"] = true;
	}
	command->initialize(values);


	if (inParallel)
	{
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommands.push_back(command);
	}
	else
	{
		cCommandGroup* cg = new cCommandGroup();
		cg->vecCommands.push_back(command);
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(cg);
	}

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

int cLuaBrain::l_MoveWithTime(lua_State * L)
{
	//int objectID = lua_tonumber(L, 1);	/* get argument */

	//// Exist? 
	//cMeshObject* pGO = cLuaBrain::m_findObjectByID(objectID);

	std::string objName = lua_tostring(L, 1);
	cMeshObject* pGO = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName(objName);

	if (pGO == nullptr)
	{	// No, it's invalid
		lua_pushboolean(L, false);
		// I pushed 1 thing on stack, so return 1;
		return 1;
	}

	// Object ID is valid
	// Get the values that lua pushed and update object
	glm::vec3 from = glm::vec3(0.0f);
	glm::vec3 to = glm::vec3(0.0f);
	from.x = lua_tonumber(L, 2);
	from.y = lua_tonumber(L, 3);
	from.z = lua_tonumber(L, 4);
	to.x = lua_tonumber(L, 5);
	to.y = lua_tonumber(L, 6);
	to.z = lua_tonumber(L, 7);
	double time = lua_tonumber(L, 8);
	bool withCamera = lua_toboolean(L, 9);

	cMoveToCommand* command = new cMoveToCommand();

	nlohmann::json values;

	values["from"]["x"] = from.x;
	values["from"]["y"] = from.y;
	values["from"]["z"] = from.z;

	values["to"]["x"] = to.x;
	values["to"]["y"] = to.y;
	values["to"]["z"] = to.z;

	//time in seconds
	values["time"] = time;
	values["withCamera"] = withCamera;
	command->setObject(pGO);
	command->initialize(values);
	

	if (inParallel)
	{
		cCommandGroup* parallelCG = cSceneUtils::getInstance()->sceneCommandGroup.findCommandGroupByFrinedlyName("parallelCG");
		parallelCG->vecCommands.push_back(command);
		//cSceneUtils::getInstance()->sceneCommandGroup.vecCommands.push_back(command);
	}
	else
	{
		cCommandGroup* cg = new cCommandGroup();
		cg->vecCommands.push_back(command);
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(cg);
	}

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

int cLuaBrain::l_OrientWithTime(lua_State *L)
{
	std::string objName = lua_tostring(L, 1);
	cMeshObject* pGO = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName(objName);

	if (pGO == nullptr)
	{	// No, it's invalid
		lua_pushboolean(L, false);
		// I pushed 1 thing on stack, so return 1;
		return 1;
	}

	// Object ID is valid
	// Get the values that lua pushed and update object
	glm::vec3 fromAngles = glm::vec3(0.0f);
	glm::vec3 toAngles = glm::vec3(0.0f);
	fromAngles.x = lua_tonumber(L, 2);
	fromAngles.y = lua_tonumber(L, 3);
	fromAngles.z = lua_tonumber(L, 4);
	toAngles.x = lua_tonumber(L, 5);
	toAngles.y = lua_tonumber(L, 6);
	toAngles.z = lua_tonumber(L, 7);
	double time = lua_tonumber(L, 8);

	cOrientToCommand* command = new cOrientToCommand();

	nlohmann::json values;

	values["fromAngles"]["x"] = fromAngles.x;
	values["fromAngles"]["y"] = fromAngles.y;
	values["fromAngles"]["z"] = fromAngles.z;

	values["toAngles"]["x"] = toAngles.x;
	values["toAngles"]["y"] = toAngles.y;
	values["toAngles"]["z"] = toAngles.z;

	//time in seconds
	values["time"] = time;
	command->setObject(pGO);
	command->initialize(values);


	if (inParallel)
	{
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommands.push_back(command);
	}
	else
	{
		cCommandGroup* cg = new cCommandGroup();
		cg->vecCommands.push_back(command);
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(cg);
	}

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

int cLuaBrain::l_FollowObject(lua_State *L)
{
	std::string objName = lua_tostring(L, 1);
	cMeshObject* pGO = NULL;
	if (objName != "camera")
	{
		pGO = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName(objName);

		if (pGO == nullptr)
		{	// No, it's invalid
			lua_pushboolean(L, false);
			// I pushed 1 thing on stack, so return 1;
			return 1;
		}
	}
	// Object ID is valid
	// Get the values that lua pushed and update object
	glm::vec3 idealLocation = glm::vec3(0.0f);
	glm::vec3 currentLocation = glm::vec3(0.0f);
	idealLocation.x = lua_tonumber(L, 2);
	idealLocation.y = lua_tonumber(L, 3);
	idealLocation.z = lua_tonumber(L, 4);
	float minDistance = lua_tonumber(L, 5);
	float maxSpeedDistance = lua_tonumber(L, 6);
	float maxSpeed = lua_tonumber(L, 7);
	currentLocation.x = lua_tonumber(L, 8);
	currentLocation.y = lua_tonumber(L, 9);
	currentLocation.z = lua_tonumber(L, 10);


	cFollowObjectCommand* command = new cFollowObjectCommand();

	nlohmann::json values;

	values["idealLocation"]["x"] = idealLocation.x;
	values["idealLocation"]["y"] = idealLocation.y;
	values["idealLocation"]["z"] = idealLocation.z;

	values["minDistance"] = minDistance;
	values["maxSpeedDistance"] = maxSpeedDistance;
	values["maxSpeed"] = maxSpeed;
	
	values["currentLocation"]["x"] = currentLocation.x;
	values["currentLocation"]["y"] = currentLocation.y;
	values["currentLocation"]["z"] = currentLocation.z;

	//time in seconds
	if (objName != "camera")
	{
		command->setObject(pGO);
		values["isCamera"] = false;
	}
	else
	{
		values["isCamera"] = true;
	}
	command->initialize(values);


	if (inParallel)
	{
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommands.push_back(command);
	}
	else
	{
		cCommandGroup* cg = new cCommandGroup();
		cg->vecCommands.push_back(command);
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(cg);
	}

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

int cLuaBrain::l_FollowCurve(lua_State *L)
{
	std::string objName = lua_tostring(L, 1);
	cMeshObject* pGO = NULL;
	pGO = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName(objName);

	if (pGO == nullptr)
	{	// No, it's invalid
		lua_pushboolean(L, false);
		// I pushed 1 thing on stack, so return 1;
		return 1;
	}
	// Object ID is valid
	// Get the values that lua pushed and update object
	std::string posWayFileName = lua_tostring(L, 2);
	std::string oriWayFileName = lua_tostring(L, 3);


	cAnimationCurveCommand* command = new cAnimationCurveCommand();

	nlohmann::json values;

	values["PositionWayPointsFileName"] = posWayFileName;
	values["OrientationWayPointsFileName"] = oriWayFileName; 

	//time in seconds
	command->setObject(pGO);
	command->initialize(values);

	if (inParallel)
	{
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommands.push_back(command);
	}
	else
	{
		cCommandGroup* cg = new cCommandGroup();
		cg->vecCommands.push_back(command);
		cSceneUtils::getInstance()->sceneCommandGroup.vecCommandGroups.push_back(cg);
	}

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing
}

/*static*/ 
std::vector< cMeshObject* >* cLuaBrain::m_p_vecGOs;


// returns nullptr if not found
/*static*/ cMeshObject* cLuaBrain::m_findObjectByID( int ID )
{
	for ( std::vector<cMeshObject*>::iterator itGO = cLuaBrain::m_p_vecGOs->begin();
		  itGO != cLuaBrain::m_p_vecGOs->end(); itGO++ )
	{
		if ( (*itGO)->getUniqueId() == ID )
		{	// Found it!
			return (*itGO);
		}
	}//for ( std::vector<cGameObject*>::iterator itGO...
	// Didn't find it
	return nullptr;
}


std::string cLuaBrain::m_decodeLuaErrorToString( int error )
{
	switch ( error )
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error"; 
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}
