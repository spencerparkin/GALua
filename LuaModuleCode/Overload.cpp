// Overload.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "lua.hpp"
#include "Overload.h"
#include "Operation.h"
#include "String.h"
#include "UserData.h"
#include "Index.h"

//=========================================================================================
static const char* userDataMetaTableName = "__galua_userdata_metatable__";

//=========================================================================================
// 1 is returned on success.  0 on failure.
// This is really a count of how many items we pushed.
int l_push_userdata_metatable( lua_State* L )
{
	// Ultimately we will find the meta-table at global scope.
	lua_pushglobaltable( L );

	// This should never take more than 2 tries.
	int tryCount = 0;
	while( true )
	{
		// Return the cached meta-table if it already exists.
		lua_getfield( L, -1, userDataMetaTableName );
		if( !lua_isnil( L, -1 ) )
		{
			// We push only the desired return value on the stack.
			lua_remove( L, -2 );
			return 1;
		}

		// Remove the nil value.
		lua_pop( L, 1 );

		// It should never take more than two tries.
		if( ++tryCount >= 2 )
			break;

		// Start a meta-table for the user-data value we'll hand back.
		lua_newtable( L );

		// Register this function so that when the value is garbage collected, we free the user-data memory.
		lua_pushcfunction( L, &DeleteGALuaUserData );
		lua_setfield( L, -2, "__gc" );

		// Register useful overloads.
		lua_pushcfunction( L, &l_sum );
		lua_setfield( L, -2, "__add" );
		lua_pushcfunction( L, &l_dif );
		lua_setfield( L, -2, "__sub" );
		lua_pushcfunction( L, &l_gp );
		lua_setfield( L, -2, "__mul" );
		lua_pushcfunction( L, &l_ip );
		lua_setfield( L, -2, "__mod" );
		lua_pushcfunction( L, &l_op );
		lua_setfield( L, -2, "__pow" );

		// Provide compatibility with the built-in "tostring" function.
		lua_pushcfunction( L, &l_to_string );
		lua_setfield( L, -2, "__tostring" );

		// These meta-methods will provide a convenient way to get and set the
		// grade parts of a multi-vector if given an integer key.  Otherwise,
		// we will use the given string key to look-up a desired user-data method.
		lua_pushcfunction( L, &l_index );
		lua_setfield( L, -2, "__index" );
		lua_pushcfunction( L, &l_newindex );
		lua_setfield( L, -2, "__newindex" );

		// Provide a convenient way to take the magnitude of and negate a multi-vector.
		lua_pushcfunction( L, &l_mag );
		lua_setfield( L, -2, "__len" );
		lua_pushcfunction( L, &l_neg );
		lua_setfield( L, -2, "__unm" );

		// Cache off the table for future use.  This also pops the table we just created.
		lua_setfield( L, -2, userDataMetaTableName );
	}
	
	// If we get here, then something went wrong!
	lua_pop( L, 1 );
	return 0;
}

// Overload.cpp