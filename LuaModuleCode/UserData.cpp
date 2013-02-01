// UserData.cpp

#include "UserData.h"

//=========================================================================================
GALuaUserData* NewGALuaUserData( lua_State* L )
{
	// Try to allocate the memory for it.
	size_t size = sizeof( GALuaUserData );
	GALuaUserData* userData = ( GALuaUserData* )lua_newuserdata( L, size );
	if( !userData )
		return 0;

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

// UserData.cpp