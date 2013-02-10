// Version.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "lua.hpp"
#include "Version.h"

//=========================================================================================
int l_version( lua_State* L )
{
	float major = ( float )GALUA_VERSION_MAJOR;
	float minor = ( float )GALUA_VERSION_MINOR;
	float version = major + minor / 10.f;

	lua_pushnumber( L, version );
	return 1;
}

// Version.cpp