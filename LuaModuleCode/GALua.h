// GALua.h

#pragma once

//=========================================================================================
#ifdef GALUA_EXPORTS
#	define GALUA_FUNC __declspec( dllexport )
#else
#	define GALUA_FUNC __declspec( dllimport )
#endif //GALUA_EXPORTS

//=========================================================================================
#include "lua.hpp"

//=========================================================================================
extern "C" {
extern GALUA_FUNC int luaopen_galua( lua_State* L );
} //extern "C"

//=========================================================================================
/*__cdecl or __stdcall? */
int l_from_string( lua_State* L );
int l_to_string( lua_State* L );

// GALua.h