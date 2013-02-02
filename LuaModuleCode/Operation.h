// BinaryOp.h

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
// Unary operations:
int l_copy( lua_State* L );
int l_negate( lua_State* L );
int l_invert( lua_State* L );
int l_reverse( lua_State* L );
int l_mag( lua_State* L );

//=========================================================================================
// Binary operations:
int l_sum( lua_State* L );
int l_dif( lua_State* L );
int l_gp( lua_State* L );
int l_ip( lua_State* L );
int l_op( lua_State* L );
int l_get_grade_part( lua_State* L );

//=========================================================================================
// Turnary operations:
int l_set_grade_part( lua_State* L );

// BinaryOp.h