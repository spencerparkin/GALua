// Definition.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "Lua.hpp"
#include "Definition.h"
#include "Environment.h"
#include "Error.h"
#include "Calculator/CalcLib.h"

//=========================================================================================
// Here we assume that a table has been pushed onto the Lua stack.
// This table should be an array of strings.  Each string is a
// basis vector element.  I'm assuming that I don't need to clean
// the stack (make it empty) before returning.
int l_def_basis( lua_State* L )
{
	// We're about to recreate the basis vector map.
	gaLuaEnv->DeleteBasisVecMap();

	// Make sure we were given an argument.  We use the top stack value and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"def_basis\" expects 1 argument." );

	// Verify that we were actually given a table.
	if( !lua_istable( L, -1 ) )
		GALuaError( L, "The function \"def_basis\" expects a table (array of strings) as its one and only argument." );

	// Iterate over the array.
	int index = 0;
	while( true )
	{
		// Index the next element.  We push the index, then the index is replaced by the indexed value.
		lua_pushinteger( L, ++index );
		lua_gettable( L, -2 );

		// We're done if we've reached the end of the array.
		if( lua_isnil( L, -1 ) )
			break;

		// We expect each element to be a string.
		if( !lua_isstring( L, -1 ) )
			GALuaError( L, "The function \"def_basis\" encountered a non-string value in the given array." );

		// Try to register the basis vector with our GA environment.
		const char* basisVec = lua_tostring( L, -1 );
		if( !gaLuaEnv->RegisterBasisVec( basisVec ) )
			GALuaError( L, "The function \"def_basis\" failed to register the basis vector \"%s\".  It may be a duplicate.\n", basisVec );

		// Pop the string value off the stack.
		lua_pop( L, 1 );
	}

	// We don't return any values on the stack.
	return 0;
}

//=========================================================================================
int l_def_sig( lua_State* L )
{
	// We are about to recreate the table, so delete it.
	gaLuaEnv->DeleteBasisVecIpTable();

	// Make sure we were given an argument.  We use the top stack value and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"def_sig\" expects 1 argument." );

	// Verify that we were actually given a function.
	if( !lua_isfunction( L, -1 ) )
		GALuaError( L, "The function \"def_sig\" expects a function as its one and only argument." );

	// Go populate the basis vector inner-product table using the given Lua function.
	int basisVecCount = gaLuaEnv->BasisVecCount();
	for( int i = 0; i < basisVecCount; i++ )
	{
		for( int j = 0; j < basisVecCount; j++ )
		{
			// Push the function and its arguments onto the stack.
			// We push the function again, because it is popped with each call.
			lua_pushvalue( L, -1 );
			lua_pushinteger( L, i + 1 );		// Notice the 1-based, not 0-based indices for Lua.
			lua_pushinteger( L, j + 1 );

			// Make the call in protected mode.
			int pcall_result = lua_pcall( L, 2, 1, 0 );
			if( pcall_result != 0 )
				GALuaError( L, "The function \"def_sig\" encountered an error while trying to evaluate the given function at (%d,%d).", i+1, j+1 );

			// Make sure we got back a number.
			if( !lua_isnumber( L, -1 ) )
				GALuaError( L, "The function \"def_sig\" expects the given function to always return a number." );

			// Try to populate the (i,j) entry of the environment's IP table.
			double scalar = lua_tonumber( L, -1 );
			if( !gaLuaEnv->SetBasisVecIpTableEntry( i, j, scalar ) )
				GALuaError( L, "The function \"def_sig\" failed to set the basis vector IP table entry at (%d,%d) to %f.", i+1, j+1, scalar );

			// Remove the returned value, setting us back to original stack state.
			lua_pop( L, 1 );
		}
	}

	// We don't return any values on the stack.
	return 0;
}

//=========================================================================================
int l_def_bar( lua_State* L )
{
	// We are about to recreate the map, so delete it.
	gaLuaEnv->DeleteBarMap();

	// Make sure we were given an argument.  We use the top stack value and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"def_bar\" expects 1 argument." );

	// Verify that we were actually given a function.
	if( !lua_isfunction( L, -1 ) )
		GALuaError( L, "The function \"def_bar\" expects a function as its one and only argument." );

	// Go populate the bar map using the given Lua function.
	int basisVecCount = gaLuaEnv->BasisVecCount();
	for( int i = 0; i < basisVecCount; i++ )
	{
		// Push the function and its arguments onto the stack.
		// We push the function again, because it is popped with each call.
		lua_pushvalue( L, -1 );
		lua_pushinteger( L, i + 1 );		// Notice the 1-based, not 0-based indices for Lua.

		// Make the call in protected mode.
		int pcall_result = lua_pcall( L, 1, 2, 0 );
		if( pcall_result != 0 )
			GALuaError( L, "The function \"def_bar\" encountered an error while trying to evaluate the given function at index %d.", i+1 );

		// Make sure we got back two numbers.
		if( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 ) )
			GALuaError( L, "The function \"def_bar\" expects the given function to return two numbers.  The first is an index, the second, a sign." );

		// Check the first argument.
		int j = lua_tointeger( L, -2 ) - 1;		// Convert Lua's 1-based index to a 0-based index.
		if( j < 0 || j > basisVecCount - 1 )
			GALuaError( L, "The function \"def_bar\" expects an index in the range [1,%d], but got %d.", basisVecCount, j+1 );

		// Check the second argument.
		int sign = lua_tointeger( L, -1 );
		if( sign != 1 && sign != -1 )
			GALuaError( L, "The function \"def_bar\" expects a sign of -1 or +1, but got %d.", sign );

		// Try to populate the map.
		if( !gaLuaEnv->BarMapSet( i, j, sign ) )
			GALuaError( L, "The function \"def_bar\" failed to set the bar map at index %d with index %d and sign %d.", i, j, sign );

		// Remove the returned values, setting us back to original stack state.
		lua_pop( L, 2 );
	}

	// We don't return any values on the stack.
	return 0;
}

// Definition.cpp