// Def.cpp

#include "Lua.hpp"
#include "Def.h"
#include "Env.h"
#include "Calculator/CalcLib.h"

//=========================================================================================
// Here we assume that a table has been pushed onto the Lua stack.
// This table should be an array of strings.  Each string is a
// basis vector element.  I'm assuming that I don't need to clean
// the stack (make it empty) before returning.
int l_def_basis( lua_State* L )
{
	char error[128];
	error[0] = '\0';
		
	try
	{
		// We're about to recreate the basis vector map.
		gaLuaEnv->DeleteBasisVecMap();

		// Verify that only one item is on the stack.
		// Our function takes one and only one argument.
		int stack_top = lua_gettop(L);
		if( stack_top != 1 )
		{
			sprintf_s( error, sizeof( error ), "The function \"def_basis\" expects 1 and only 1 argument, not %d arguments.", stack_top );
			throw( error );
		}

		// Verify that we were actually given a table.
		if( !lua_istable( L, -1 ) )
		{
			strcpy_s( error, sizeof( error ), "The function \"def_basis\" expects a table (array of strings) as its one and only argument." );
			throw( error );
		}

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
			{
				strcpy_s( error, sizeof( error ), "The function \"def_basis\" encountered a non-string value in the given array." );
				throw( error );
			}

			// Try to register the basis vector with our GA environment.
			const char* basisVec = lua_tostring( L, -1 );
			if( !gaLuaEnv->RegisterBasisVec( basisVec ) )
			{
				sprintf_s( error, sizeof( error ), "The function \"def_basis\" failed to register the basis vector \"%s\".  It may be a duplicate.\n", basisVec );
				throw( error );
			}

			// Pop the string value off the stack.
			lua_pop( L, 1 );
		}
	}
	catch( const char* )
	{
	}

	// If something went wrong, tell Lua.
	if( error[0] != '\0' )
		luaL_error( L, error );

	// We don't return any values on the stack.
	return 0;
}

//=========================================================================================
int l_def_sig( lua_State* L )
{
	char error[256];
	error[0] = '\0';
		
	try
	{
		// We are about to recreate the table, so delete it.
		gaLuaEnv->DeleteBasisVecIpTable();

		// Verify that only one item is on the stack.
		// Our function takes one and only one argument.
		int stack_top = lua_gettop(L);
		if( stack_top != 1 )
		{
			sprintf_s( error, sizeof( error ), "The function \"def_dig\" expects 1 and only 1 argument, not %d arguments.", stack_top );
			throw( error );
		}

		// Verify that we were actually given a function.
		if( !lua_isfunction( L, -1 ) )
		{
			strcpy_s( error, sizeof( error ), "The function \"def_sig\" expects a function as its one and only argument." );
			throw( error );
		}

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
				// TODO: How do I get the error information from Lua for this call if this call fails?
				int pcall_result = lua_pcall( L, 2, 1, 0 );
				if( pcall_result != 0 )
				{
					sprintf_s( error, sizeof( error ), "The function \"def_sig\" encountered an error while trying to evaluate the given function at (%d,%d).", i, j );
					throw( error );
				}

				// Make sure we got back a number.
				if( !lua_isnumber( L, -1 ) )
				{
					strcpy_s( error, sizeof( error ), "The function \"def_sig\" expects the given function to always return a number." );
					throw( error );
				}

				// Try to populate the (i,j) entry of the environment's IP table.
				double scalar = lua_tonumber( L, -1 );
				if( !gaLuaEnv->SetBasisVecIpTableEntry( i, j, scalar ) )
				{
					sprintf_s( error, sizeof( error ), "The function \"def_sig\" failed to set the basis vector IP table entry at (%d,%d) to %f.", i, j, scalar );
					throw( error );
				}

				// Remove the returned value, setting us back to original stack state.
				lua_pop( L, 1 );
			}
		}
	}
	catch(...)
	{
	}

	// If something went wrong, tell Lua.
	if( error[0] != '\0' )
		luaL_error( L, error );

	// We don't return any values on the stack.
	return 0;
}

// Def.cpp