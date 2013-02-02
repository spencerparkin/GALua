// BinaryOp.h

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