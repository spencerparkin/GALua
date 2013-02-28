// Convert.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "lua.hpp"
#include "Compare.h"
#include "UserData.h"
#include "Error.h"

//=========================================================================================
// Do nothing more than return our given argument.  This converts the argument
// into a multi-vector value, because we use coercion.
int l_convert( lua_State* L )
{
	// Make sure we were given an argument.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"convert\" expects an argument." );

	// Grab our argument with potential coercion.
	bool coercedUserData = false;
	GALuaUserData* argUserData = 0;
	argUserData = GrabGALuaUserData( L, -1, &coercedUserData );
	if( !argUserData )
		GALuaError( L, "The function \"convert\" failed to grab its argument." );

	// If coercion happened, remove the original argument.
	if( coercedUserData )
		lua_remove( L, -2 );

	// Return our argument.
	return 1;
}

// Convert.cpp