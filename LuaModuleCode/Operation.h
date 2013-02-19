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
enum GALuaOp
{
	// Unary operations:
	UNARY_OP_COPY,
	UNARY_OP_NEGATE,
	UNARY_OP_INVERT_LEFT,
	UNARY_OP_INVERT_RIGHT,
	UNARY_OP_REVERSE,
	UNARY_OP_MAGNITUDE,
	UNARY_OP_BAR,

	// Binary operations:
	BINARY_OP_SUM,
	BINARY_OP_DIF,
	BINARY_OP_GP,
	BINARY_OP_IP,
	BINARY_OP_OP,
	BINARY_OP_GP_INV,
	BINARY_OP_COEF,
	BINARY_OP_GET_GRADE_PART,

	// Turnary operations:
	TURNARY_OP_SET_GRADE_PART,
};

//=========================================================================================
int PerformOp( lua_State* L, GALuaOp gaLuaOp );

//=========================================================================================
// Unary operations:
int l_copy( lua_State* L );
int l_neg( lua_State* L );
int l_left_inverse( lua_State* L );
int l_right_inverse( lua_State* L );
int l_reverse( lua_State* L );
int l_mag( lua_State* L );
int l_bar( lua_State* L );

//=========================================================================================
// Binary operations:
int l_sum( lua_State* L );
int l_dif( lua_State* L );
int l_gp( lua_State* L );
int l_ip( lua_State* L );
int l_op( lua_State* L );
int l_gp_inv( lua_State* L );
int l_coef( lua_State* L );
int l_get_grade_part( lua_State* L );

//=========================================================================================
// Turnary operations:
int l_set_grade_part( lua_State* L );

// BinaryOp.h