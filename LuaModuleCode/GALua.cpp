// GALua.cpp

#include <windows.h>
#include "GALua.h"
#include "Env.h"
#include "Def.h"
#include "String.h"

//=========================================================================================
BOOL APIENTRY DllMain( HANDLE module, DWORD reason, LPVOID reserved )
{
	switch( reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			gaLuaEnv = new GALuaEnv();
			if( !gaLuaEnv )
				return FALSE;
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			delete gaLuaEnv;
			gaLuaEnv = 0;
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
	{ "def_basis", l_def_basis },
	{ "def_sig", l_def_sig },
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

// GALua.cpp