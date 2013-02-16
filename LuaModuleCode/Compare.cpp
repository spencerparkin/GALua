// Compare.cpp

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
int l_cmp_eq( lua_State* L )
{
	// Make sure we were given at least the desired number of arguments.  We use the top stack values and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 2 )
		GALuaError( L, "The function \"eq\" expects 2 arguments." );

	// Try to grab our arguments.  Don't bother with coercion for simplicity for now.
	GALuaUserData* argUserData[2];
	argUserData[0] = GrabGALuaUserData( L, -2 );
	if( !argUserData[0] )
		GALuaError( L, "The function \"eq\" failed to grab its first argument." );
	argUserData[1] = GrabGALuaUserData( L, -1 );
	if( !argUserData[1] )
		GALuaError( L, "The function \"eq\" failed to grab its second argument." );

	// Find the square magnitude of the difference between the two multi-vectors.
	GeometricAlgebra::SumOfBlades difference;
	if( !difference.AssignDifference( *argUserData[0]->multiVec, *argUserData[1]->multiVec ) )
		GALuaError( L, "The function \"eq\" encountered an internal error while performing a difference operation." );
	GeometricAlgebra::Scalar scalar;
	if( !difference.AssignSquareMagnitudeTo( scalar ) )
		GALuaError( L, "The function \"eq\" encountered an internal error while trying to grab the square magnitude of a difference." );
	
	// For now, any error generated here is ignored, unfortunately.
	double squareMag = scalar;

	// Determine equality.
	bool equal = false;
	if( squareMag == 0.0 )
		equal = true;

	// Pop our given arguments.  This is not a requirement when being called
	// from Lua, but if we want to call this function internally, it's a good
	// convention to have.
	lua_pop( L, 2 );

	// Push our result and return 1 to indicate how many return-values we left on the stack.
	lua_pushboolean( L, equal );
	return 1;
}

// Compare.cpp