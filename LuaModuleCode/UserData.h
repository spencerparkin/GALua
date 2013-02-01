// UserData.h

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
GALuaUserData* GrabGALuaUserData( lua_State* L, int idx );

// UserData.h