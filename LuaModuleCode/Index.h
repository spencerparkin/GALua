// Index.h

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
// The idea for these functions comes from Josh.
// The idea here is to let us overload the square-brackets
// syntax while also being able to provide support for the
// object-oriented syntax with user-data methods.
int l_index( lua_State* L );
int l_newindex( lua_State* L );

// Index.h