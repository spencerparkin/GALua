// GALua.h

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#pragma once

//=========================================================================================
#ifdef GALUA_EXPORTS
#	define GALUA_FUNC __declspec( dllexport )
#else
#	define GALUA_FUNC __declspec( dllimport )
#endif //GALUA_EXPORTS

//=========================================================================================
// This is the C++ include for Lua stuff.
#include "lua.hpp"

//=========================================================================================
// Lua will grab this symbol from our DLL and call it to open our library module.
extern "C" {
extern GALUA_FUNC int luaopen_galua( lua_State* L );
} //extern "C"

// GALua.h