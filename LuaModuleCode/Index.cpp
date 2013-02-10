// Index.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "Index.h"
#include "Operation.h"
#include "Scope.h"
#include "String.h"
#include <string.h>

//=========================================================================================
// -2 -> self
// -1 -> key
int l_index( lua_State* L )
{
	// If given an integer, get the appropriate part of the multi-vector.
	if( lua_isnumber( L, -1 ) )
		return l_get_grade_part(L);
	
	// If given a string, try to identify it as a known user-data method.
	if( lua_isstring( L, -1 ) )
	{
		// Do we recognize the desired user-data method?
		// If the list of functions here gets too big, we should consider
		// putting the function in a table and then doing a table look-up.
		const char* key = lua_tostring( L, -1 );
		if( 0 == strcmp( key, "copy" ) )
		{
			lua_pushcfunction( L, &l_copy );
			return 1;
		}
		else if( 0 == strcmp( key, "bar" ) )
		{
			lua_pushcfunction( L, &l_bar );
			return 1;
		}
		else if( 0 == strcmp( key, "latex" ) )
		{
			lua_pushcfunction( L, &l_to_latex_string );
			return 1;
		}

		// If we didn't recognize the method, it may be a variable containing a
		// multi-vector, in which case we can use the dot operator as the inner product.
		if( 1 == l_scope_lookup( L, key ) && lua_isuserdata( L, -1 ) )
		{
			lua_remove( L, -2 );
			return l_ip(L);
		}
	}

	// If given user-data, try to perform the inner product.
	if( lua_isuserdata( L, -1 ) )
		return l_ip(L);

	// The fall-back case is to return nil.
	lua_pushnil(L);
	return 1;
}

//=========================================================================================
// -3 -> self
// -2 -> key
// -1 -> value
int l_newindex( lua_State* L )
{
	// If given an integer, set the appropriate part of the multi-vector.
	if( lua_isnumber( L, -2 ) )
		return l_set_grade_part(L);

	// The fall-back case is to return nil.
	lua_pushnil(L);
	return 1;
}

// Index.cpp