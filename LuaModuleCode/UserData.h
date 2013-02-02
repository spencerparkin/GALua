// UserData.h

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#pragma once

//=========================================================================================
#include "lua.hpp"
#include "Calculator/CalcLib.h"

//=========================================================================================
struct GALuaUserData
{
	char safetyString[5];
	GeometricAlgebra::SumOfBlades* multiVec;
};

//=========================================================================================
GALuaUserData* NewGALuaUserData( lua_State* L );
GALuaUserData* GrabGALuaUserData( lua_State* L, int idx, bool* coercedUserData = 0 );
int DeleteGALuaUserData( lua_State* L );

// UserData.h