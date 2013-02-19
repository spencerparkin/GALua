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
#include <string.h>

//=========================================================================================
// Here we are trying to synthesize Lua's scoping algorithm.
// This is a dubious thing to do, because we may not get it right,
// and even if we do get it right, if Lua's scoping algorithm ever
// changes, this code becomes invalid.  The use of this function
// is therefore limited to only one special case that I'm handling,
// because I want to.
//
// It should also be noted that since this function utilizes the
// debug library, it may be a performance bottle-neck.  Module users
// can avoid this performance hit by not using the any feature of
// the module that does a scope look-up, which, at the time of this
// writing, is confined to a single special case of supporting the
// dot operator syntax for the inner product.
//
// Push onto the stack the value of the given variable at the current scope.
// Push nil if it does not exist.
int l_scope_lookup( lua_State* L, const char* varName )
{
	// If we can get the activation record information, we can
	// search the currently visible locals and up-values.
	lua_Debug ar;
	if( 1 == lua_getstack( L, 0, &ar ) )
	{
		// Hunt for visible local variables by the given name.
		int i = 1, j = 0;
		while( true )
		{
			const char* localVarName = lua_getlocal( L, &ar, i++ );
			if( !localVarName )
				break;
			if( 0 == strcmp( localVarName, varName ) )
				j++;
			else
				lua_pop( L, 1 );
		}

		// Did we find one or more matches?  If so, then the last
		// one found overrides the visibility of the rest.
		while( j > 1 )
		{
			lua_remove( L, -2 );
			j--;
		}
		if( j == 1 )
			return 1;

		// Push onto the stack the calling function.
		if( 0 != lua_getinfo( L, "f", &ar ) )
		{
			// Hunt for visible up-values by the given name.
			// No two up-values can have the same name, so we can't stop when we've found it.
			i = 1;
			while( true )
			{
				const char* upVarName = lua_getupvalue( L, -1, i++ );
				if( !upVarName )
					break;
				if( 0 == strcmp( upVarName, varName ) )
				{
					// Return just the up-value.
					lua_remove( L, -2 );
					return 1;
				}
				else
					lua_pop( L, 1 );
			}

			// We didn't find the up-value.  Remove the calling function from the stack.
			lua_pop( L, 1 );
		}
	}

	// Will this call check the calling function's environment table?
	// Our last resort is always to search the set of global variables.
	lua_getglobal( L, varName );
	return 1;
}

// Scope.cpp