// Scope.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "Scope.h"

//=========================================================================================
// Here we are trying to synthesize Lua's scoping algorithm.
// This is a dubious thing to do, because we may not get it right,
// and even if we do get it right, if Lua's scoping algorithm ever
// changes, this code becomes invalid.  The use of this function
// is therefore limited to only one special case that I'm handling,
// because I want to.
//
// Push onto the stack the value of the given variable at the current scope.
// Push nil if it does not exist.
int l_scope_lookup( lua_State* L, const char* varName )
{
	// TODO: Find local variables by using the debug library stuff?
	lua_getglobal( L, varName );
	return 1;
}

// Scope.cpp