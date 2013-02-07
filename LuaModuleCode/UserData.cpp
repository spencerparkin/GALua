// UserData.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "UserData.h"
#include "Operation.h"
#include "String.h"
#include "Debug.h"

//=========================================================================================
GALuaUserData* NewGALuaUserData( lua_State* L )
{
	// Try to allocate the memory for it.
	size_t size = sizeof( GALuaUserData );
	GALuaUserData* userData = ( GALuaUserData* )lua_newuserdata( L, size );
	if( !userData )
		return 0;

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

	// Provide a convenient way to get and set the grade parts of a multi-vector.
	lua_pushcfunction( L, &l_get_grade_part );
	lua_setfield( L, -2, "__index" );
	lua_pushcfunction( L, &l_set_grade_part );
	lua_setfield( L, -2, "__newindex" );

	// Provide a convenient way to take the magnitude of and negate a multi-vector.
	lua_pushcfunction( L, &l_mag );
	lua_setfield( L, -2, "__len" );
	lua_pushcfunction( L, &l_negate );
	lua_setfield( L, -2, "__unm" );

	// Finally, set the meta-table.  This also pops it, leaving only
	// what we want to return from this function at the top of the stack.
	lua_setmetatable( L, -2 );

	// Fill out the user-data with the correct initial data.
	memcpy( userData->safetyString, "gaLua", 5 );
	userData->multiVec = 0;
	
	// Kuplah, Worf!
	return userData;
}

//=========================================================================================
// If the coercion pointer is not given, then coercion is not allowed!
// If coercion happens, we should be adding the coerced value on the
// top of the stack and nothing else.
GALuaUserData* GrabGALuaUserData( lua_State* L, int idx, bool* coercedUserData /*= 0*/ )
{
	// Ultimately return this.
	GALuaUserData* userData = 0;

	// Assume that no type coercion will occur.
	if( coercedUserData )
		*coercedUserData = false;

	// Do we have user-data at the given stack location?
	if( lua_isuserdata( L, idx ) )
	{
		// Make sure we can get a pointer to it.
		userData = ( GALuaUserData* )lua_touserdata( L, idx );
		if( userData )
		{
			// It is likely that if the following safety check passes, then
			// the user data we were given is the type of user data our
			// module is designed to handle.
			char safetyString[6];
			strncpy_s( safetyString, sizeof( safetyString ), userData->safetyString, 5 );
			safetyString[5] = '\0';
			if( 0 != strcmp( safetyString, "gaLua" ) )
				userData = 0;	// When the stack gets cleaned up, this will get cleaned up, so don't feel bad about losing our pointer to the data.
		}
	}
	else if( coercedUserData )	// Is coercion allowed?
	{
		// Yes.  We can coerce numbers and strings.
		if( lua_isnumber( L, idx ) )
		{
			// In the case of numbers, we can always coerce the data-type into our user-data type.
			*coercedUserData = true;
			GeometricAlgebra::Scalar scalar = lua_tonumber( L, idx );
			userData = NewGALuaUserData( L );
			if( userData )
			{
				userData->multiVec = new GeometricAlgebra::SumOfBlades();
				userData->multiVec->AssignScalar( scalar );
			}
		}
		else if( lua_isstring( L, idx ) )
		{
			// In the case of strings, we might be able to coerce the data-type into our user-data type.
			*coercedUserData = true;
			lua_pushvalue( L, idx );	// Getting read for l_from_string, make sure it's at the stack top.
			l_from_string( L );			// If this fails, the entire script fails, so no need to check return value.
			lua_remove( L, -2 );		// Make sure that our only addition to the stack is the coerced value.
			userData = GrabGALuaUserData( L, -1 );		// Find our user-data at the top of the stack.
		}
	}

	// Let'em have it!
	return userData;
}

//=========================================================================================
int DeleteGALuaUserData( lua_State* L )
{
	GALuaUserData* userData = GrabGALuaUserData( L, -1 );
	if( userData )
	{
		delete userData->multiVec;
		userData->multiVec = 0;
	}

	return 0;
}

// UserData.cpp