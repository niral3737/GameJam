#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua5.3.3\lua.h>
#include <Lua5.3.3\lua.h>
#include <Lua5.3.3\lauxlib.h>
#include <Lua5.3.3/lualib.h>
}

#include <string>
#include <vector>
#include "cMeshObject.h"
#include <map>
#include "cCommandGroup.h"

class cLuaBrain
{
public:
	// Init Lua and set callback functions
	cLuaBrain();
	~cLuaBrain();
	void RunThis(std::string theLuaScript);
	void LoadScript( std::string scriptName, 
					 std::string scriptSource );
	void DeleteScript( std::string scriptName );
	// Passes a pointer to the game object vector
	void SetObjectVector( std::vector< cMeshObject* >* p_vecGOs );
	// Call all the active scripts that are loaded
	void Update(float deltaTime);

	static int l_KillAllHumans( lua_State *L );
	// Called by Lua
	// Passes object ID, new velocity, etc.
	// Returns valid (true or false)
	// Passes: 
	// - position (xyz)
	// - velocity (xyz)
	// called "setObjectState" in lua
	static int l_UpdateObject( lua_State *L );		// c function
	// Passes object ID
	// Returns valid (true or false)
	// - position (xyz)
	// - velocity (xyz)
	// called "getObjectState" in lua
	static int l_GetObjectState( lua_State *L );
	static int l_MoveWithTime(lua_State *L);
	static int l_OrientWithTime(lua_State *L);
	static int l_FollowObject(lua_State *L);
	static int l_FollowCurve(lua_State *L);
	static int l_parallel(lua_State *L);
	static int l_MoveToLookAt(lua_State *L);
	static int l_LocationTriggerMoveOrient(lua_State *L);

	static cMeshObject* pMyGameObject;

private:
	std::map< std::string /*scriptName*/, 
			  std::string /*scriptSource*/ > m_mapScripts;

	static std::vector< cMeshObject* >* m_p_vecGOs;
	// returns nullptr if not found
	static cMeshObject* m_findObjectByID( int ID );
	static bool inParallel;

	lua_State* m_pLuaState;

	std::string m_decodeLuaErrorToString( int error );
};

#endif
