// GALua.cpp

#include <windows.h>
#include "GALua.h"
#include "Env.h"
#include "Def.h"
#include "String.h"
#include "BinaryOp.h"
#include "UnaryOp.h"

//=========================================================================================
// By avoiding the use of DllMain, our Lua module has the potential to be portable to other platforms.
#if 0
BOOL APIENTRY DllMain( HANDLE module, DWORD reason, LPVOID reserved )
{
	return TRUE;
}
#endif

//=========================================================================================
static luaL_Reg galua_api[] =
{
	{ "def_basis", l_def_basis },
	{ "def_sig", l_def_sig },
	{ "from_string", l_from_string },
	{ "to_string", l_to_string },
	{ "sum", l_sum },
	{ "gp", l_gp },
	{ "ip", l_ip },
	{ "op", l_op },
	{ "copy", l_copy },
	{ "negate", l_negate },
	{ "invert", l_invert },
	{ "reverse", l_reverse },
	{ "mag", l_mag },
	{ NULL, NULL },
};

//=========================================================================================
static void ModuleInitialize( void )
{
	// Create our GA calculation environment.
	gaLuaEnv = new GALuaEnv();
}

//=========================================================================================
static void ModuleFinalize( void )
{
	delete gaLuaEnv;
	gaLuaEnv = 0;
}

//=========================================================================================
// Unfortunately, Lua doesn't call this itself.  We have to rig it into the system.
int luaclose_galua( lua_State* L )
{
	ModuleFinalize();

	return 0;
}

//=========================================================================================
// Lua will find this symbol and call it to open our library module.
extern "C" {
GALUA_FUNC int luaopen_galua( lua_State* L )
{
	ModuleInitialize();

	luaL_newlib( L, galua_api );

	// Using the API table in function syntax should be a nice way to create multi-vectors.
	lua_newtable( L );
	lua_pushcfunction( L, &luaclose_galua );
	lua_setfield( L, -2, "__gc" );
	lua_pushcfunction( L, &l_from_string );
	lua_setfield( L, -2, "__call" );
	lua_setmetatable( L, -2 );

	return 1;
}
} //extern "C"

// GALua.cpp