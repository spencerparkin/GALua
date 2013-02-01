// GALua.cpp

#include <windows.h>
#include "GALua.h"

//=========================================================================================
BOOL APIENTRY DllMain( HANDLE module, DWORD reason, LPVOID reserved )
{
	switch( reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	return TRUE;
}

//=========================================================================================
static luaL_Reg galua[] =
{
	{ "from_string", l_from_string },
	{ "to_string", l_to_string },
	{ NULL, NULL },
};

//=========================================================================================
extern "C" {
GALUA_FUNC int luaopen_galua( lua_State* L )
{
	luaL_newlib( L, galua );
	return 1;
}
} //extern "C"

//=========================================================================================
int l_from_string( lua_State* L )
{
	lua_pushnumber( L, 5 );
	return 1;
}

//=========================================================================================
int l_to_string( lua_State* L )
{
	lua_pushstring( L, "test" );
	return 1;
}

// GALua.cpp