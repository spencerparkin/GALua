// Scope.h

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

//=========================================================================================
int l_scope_lookup( lua_State* L, const char* varName );

// Scope.h