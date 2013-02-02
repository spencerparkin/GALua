// UserData.cpp

#include "UserData.h"
#include "UnaryOp.h"
#include "BinaryOp.h"
#include "String.h"

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

	// TODO: Have __index and __newindex let you set and get grade parts of the multi-vector.
	// TODO: Have __eq let you compare multi-vectors.

	// TODO: Figure this out.  It doesn't work.  What are the two args?
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
GALuaUserData* GrabGALuaUserData( lua_State* L, int idx )
{
	// Make sure that user data exists at the given stack location.
	if( !lua_isuserdata( L, idx ) )
		return 0;
	
	// Make sure we can get a pointer to it.
	GALuaUserData* userData = ( GALuaUserData* )lua_touserdata( L, idx );
	if( !userData )
		return 0;

	// It is likely that if the following safety check fails, then
	// the user data we were given is the type of user data our
	// module is designed to handle.
	char safetyString[6];
	strncpy_s( safetyString, sizeof( safetyString ), userData->safetyString, 5 );
	safetyString[5] = '\0';
	if( 0 != strcmp( safetyString, "gaLua" ) )
		return 0;

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