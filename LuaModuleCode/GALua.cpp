// GALua.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include <windows.h>
#include "GALua.h"
#include "Environment.h"
#include "Definition.h"
#include "String.h"
#include "Number.h"
#include "BasisVec.h"
#include "Operation.h"
#include "Version.h"

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
	{ "version", l_version },
	{ "def_basis", l_def_basis },
	{ "def_sig", l_def_sig },
	{ "def_bar", l_def_bar },
	{ "from_string", l_from_string },
	{ "to_string", l_to_string },
	{ "to_latex_string", l_to_latex_string },
	{ "from_number", l_from_number },
	{ "to_number", l_to_number },
	{ "sum", l_sum },
	{ "add", l_sum },
	{ "dif", l_dif },
	{ "sub", l_dif },
	{ "gp", l_gp },
	{ "ip", l_ip },
	{ "op", l_op },
	{ "gp_inv", l_gp_inv },
	{ "coef", l_coef },
	{ "get_grade_part", l_get_grade_part },
	{ "set_grade_part", l_set_grade_part },
	{ "copy", l_copy },
	{ "neg", l_neg },
	{ "inverse", l_right_inverse },
	{ "right_inverse", l_right_inverse },
	{ "left_inverse", l_left_inverse },
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

	// After the environment has been blown away, there
	// should be no more references to basis vector names.
	GALuaBasisVec::WipeBasisVecNameCache();
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