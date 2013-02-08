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
		const char* key = lua_tostring( L, -1 );
		if( 0 == strcmp( key, "copy" ) )
		{
			lua_pushcfunction( L, &l_copy );
			return 1;
		}
		//else...More user-data methods go here...
	}

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