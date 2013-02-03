// Operation.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include "Lua.hpp"
#include "Operation.h"
#include "UserData.h"
#include "Error.h"
#include "Calculator/CalcLib.h"

//=========================================================================================
enum GALuaOp
{
	// Unary operations:
	UNARY_OP_COPY,
	UNARY_OP_NEGATE,
	UNARY_OP_INVERT,
	UNARY_OP_REVERSE,
	UNARY_OP_MAGNITUDE,

	// Binary operations:
	BINARY_OP_SUM,
	BINARY_OP_DIF,
	BINARY_OP_GP,
	BINARY_OP_IP,
	BINARY_OP_OP,
	BINARY_OP_GET_GRADE_PART,

	// Turnary operations:
	TURNARY_OP_SET_GRADE_PART,
};

//=========================================================================================
// In following with Lua design principles, this function operates on top
// stack values, replacing them in the stack with the result of operating upon them.
static int PerformOp( lua_State* L, const char* funcName, GALuaOp binaryOp, int argCount )
{
	GALuaUserData** argUserData = 0;
	GeometricAlgebra::SumOfBlades* opResult = 0;

	// Operations take one or more arguments.
	if( argCount <= 0 )
		GALuaError( L, "The function \"%s\" doesn't take any arguments, yet is expected to grab arguments.", funcName );

	// Make sure we were given at least the desired number of arguments.  We use the top stack values and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < argCount )
		GALuaError( L, "The function \"%s\" expects %d argument(s).", funcName, argCount );

	// Allocate room for our arguments and create a map of where they are on the Lua stack.
	argUserData = new GALuaUserData*[ argCount ];
	for( int i = 0; i < argCount; i++ )
		argUserData[i] = 0;

	// Try to go grab all of our arguments.
	int offset = 0;
	for( int index = 0; index < argCount; index++ )
	{
		bool coercedUserData = false;
		int argIdx = -argCount + index - offset;
		argUserData[ index ] = GrabGALuaUserData( L, argIdx, &coercedUserData );
		if( !argUserData[ index ] )
			GALuaError( L, "The function \"%s\" failed to grab argument %d.  Is it the right type?", funcName, index + 1 );

		// If coercion occurred, we must adjust our knowledge of the stack location of each argument relative to the top of the stack.
		// This is because the coerced user-data value is always pushed onto the top of the stack.
		if( coercedUserData )
			offset++;
	}

	// Try to allocate memory for the result.
	opResult = new GeometricAlgebra::SumOfBlades();
	if( !opResult )
		GALuaError( L, "The function \"%s\" failed to allocate memory for the result.", funcName );

	// Try to perform the desired operation.
	bool operationPerformed = false;
	switch( binaryOp )
	{
		case UNARY_OP_COPY:
		{
			operationPerformed = opResult->AssignSumOfBlades( *argUserData[0]->multiVec );
			break;
		}
		case UNARY_OP_NEGATE:
		{
			operationPerformed = opResult->AssignSumOfBlades( *argUserData[0]->multiVec );
			if( operationPerformed )
				operationPerformed = opResult->Scale( GeometricAlgebra::Scalar( -1.0 ) );
			break;
		}
		case UNARY_OP_INVERT:
		{
			operationPerformed = false;

			// TODO: This algorithm needs to be revisited.
			//operationPerformed = opResult->AssignGeometricInverse( *argUserData[0]->multiVec );
			break;
		}
		case UNARY_OP_REVERSE:
		{
			operationPerformed = opResult->AssignSumOfBlades( *argUserData[0]->multiVec );
			if( operationPerformed )
				operationPerformed = opResult->Reverse();
			break;
		}
		case UNARY_OP_MAGNITUDE:
		{
			GeometricAlgebra::Scalar squareMagnitude;
			operationPerformed = argUserData[0]->multiVec->AssignSquareMagnitudeTo( squareMagnitude );
			if( operationPerformed )
			{
				double magnitude = sqrt( squareMagnitude );
				operationPerformed = opResult->AssignScalarFrom( magnitude );
			}
			break;
		}
		case BINARY_OP_SUM:
		{
			operationPerformed = opResult->AssignSum( *argUserData[0]->multiVec, *argUserData[1]->multiVec );
			break;
		}
		case BINARY_OP_DIF:
		{
			operationPerformed = opResult->AssignDifference( *argUserData[0]->multiVec, *argUserData[1]->multiVec );
			break;
		}
		case BINARY_OP_GP:
		{
			operationPerformed = opResult->AssignGeometricProduct( *argUserData[0]->multiVec, *argUserData[1]->multiVec );
			break;
		}
		case BINARY_OP_IP:
		{
			operationPerformed = opResult->AssignInnerProduct( *argUserData[0]->multiVec, *argUserData[1]->multiVec );
			break;
		}
		case BINARY_OP_OP:
		{
			operationPerformed = opResult->AssignOuterProduct( *argUserData[0]->multiVec, *argUserData[1]->multiVec );
			break;
		}
		case BINARY_OP_GET_GRADE_PART:
		{
			// TODO: This has bugs.  Fix it.
			GeometricAlgebra::Scalar scalar;
			operationPerformed = argUserData[0]->multiVec->AssignScalar( scalar );
			if( operationPerformed )
			{
				int gradePart = int( scalar );
				operationPerformed = argUserData[1]->multiVec->GradePart( gradePart, *opResult );
			}
			break;
		}
		case TURNARY_OP_SET_GRADE_PART:
		{
			break;
		}
	}

	// Did it work?!
	if( !operationPerformed )
		GALuaError( L, "The function \"%s\" failed to perform the desired operation.", funcName );

	// Create some user data for the result.
	GALuaUserData* resultUserData = NewGALuaUserData(L);
	if( !resultUserData )
		GALuaError( L, "The function \"%s\" failed to create Lua user-data for the result.", funcName );

	// Save off the pointer in Lua.
	resultUserData->multiVec = opResult;

	// Clean up our mess, if any.
	if( argUserData )
		delete[] argUserData;

	// We return one argument.  This counts values starting from the top of the stack,
	// so the stack doesn't have to contain our returned value(s) exclusively.
	// We may have added more stack items, but only the top is returned.
	// In other cases it is important that we follow a convention that keeps
	// the stack clean of any temporary values, but here we're fine.  In any case,
	// any function that manipulates the Lua stack should be well defined so that
	// callers can know the expected behavior.
	return 1;
}

//=========================================================================================
int l_copy( lua_State* L )
{
	return PerformOp( L, "copy", UNARY_OP_COPY, 1 );
}

//=========================================================================================
int l_negate( lua_State* L )
{
	return PerformOp( L, "negate", UNARY_OP_NEGATE, 1 );
}

//=========================================================================================
int l_invert( lua_State* L )
{
	return PerformOp( L, "invert", UNARY_OP_INVERT, 1 );
}

//=========================================================================================
int l_reverse( lua_State* L )
{
	return PerformOp( L, "reverse", UNARY_OP_REVERSE, 1 );
}

//=========================================================================================
int l_mag( lua_State* L )
{
	return PerformOp( L, "mag", UNARY_OP_MAGNITUDE, 1 );
}

//=========================================================================================
int l_sum( lua_State* L )
{
	return PerformOp( L, "sum", BINARY_OP_SUM, 2 );
}

//=========================================================================================
int l_dif( lua_State* L )
{
	return PerformOp( L, "dif", BINARY_OP_DIF, 2 );
}

//=========================================================================================
int l_gp( lua_State* L )
{
	return PerformOp( L, "gp", BINARY_OP_GP, 2 );
}

//=========================================================================================
int l_ip( lua_State* L )
{
	return PerformOp( L, "ip", BINARY_OP_IP, 2 );
}

//=========================================================================================
int l_op( lua_State* L )
{
	return PerformOp( L, "op", BINARY_OP_OP, 2 );
}

//=========================================================================================
int l_get_grade_part( lua_State* L )
{
	return PerformOp( L, "grade_part", BINARY_OP_GET_GRADE_PART, 2 );
}

//=========================================================================================
int l_set_grade_part( lua_State* L )
{
	return PerformOp( L, "set_grade_part", TURNARY_OP_SET_GRADE_PART, 3 );
}

// Operation.cpp