// Number.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

//=========================================================================================
#include "lua.hpp"
#include "Number.h"
#include "UserData.h"
#include "Error.h"

//=========================================================================================
int l_from_number( lua_State* L )
{
	// Make sure we were given an argument.  We will only use the top value in the stack and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"from_number\" expects an argument." );

	// Grab the scalar and remove it from the stack in keeping with our calling conventions.
	GeometricAlgebra::Scalar scalar = lua_tonumber( L, -1 );
	lua_remove( L, -1 );

	// Push a new multi-vector value onto the stack and fill-out its value.
	GALuaUserData* userData = NewGALuaUserData( L );
	if( !userData )
		GALuaError( L, "The function \"from_number\" failed to create a new multi-vector value on the stack." );
	userData->multiVec = new GeometricAlgebra::SumOfBlades();
	userData->multiVec->AssignScalar( scalar );

	// We return one result.
	return 1;
}

//=========================================================================================
// Of course, all scalars are multi-vectors, but not all multi-vectors are scalars.
// In the case that the given multi-vector is not a scalar, I think we should fail fataly.
int l_to_number( lua_State* L )
{
	// Make sure we were given an argument.  We will only use the top value in the stack and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"to_number\" expects an argument." );

	// Grab our multi-vector and remove it from the stack in keeping with our calilng conventions.
	GALuaUserData* argUserData = GrabGALuaUserData( L, -1 );
	if( !argUserData )
		GALuaError( L, "The function \"to_number\" failed to grab its argument as a multi-vector." );
	lua_remove( L, -1 );

	// Fail if the multi-vector is not actually a scalar.
	if( !argUserData->multiVec->IsHomogeneousOfGrade(0) )
		GALuaError( L, "The function \"to_number\" requires its multi-vector argument to be homogeneous of grade zero." );

	// Do the conversion from scalar to Lua number.
	double scalar;
	if( !argUserData->multiVec->AssignScalarTo( scalar ) )
		GALuaError( L, "The function \"to_number\" encountered an internal error while trying to convert a scalar to a Lua number." );
	lua_pushnumber( L, scalar );

	// We return one result.
	return 1;
}

// Number.cpp