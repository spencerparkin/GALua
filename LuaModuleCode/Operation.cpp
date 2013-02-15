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
// In following with Lua design principles, this function operates on top
// stack values, replacing them in the stack with the result of operating upon them.
int PerformOp( lua_State* L, GALuaOp gaLuaOp )
{
	// Translate the given enum into the name of the Lua function that got called.
	const char* funcName = "???";
	switch( gaLuaOp )
	{
		case UNARY_OP_COPY:					funcName = "copy";				break;
		case UNARY_OP_NEGATE:				funcName = "negate";			break;
		case UNARY_OP_INVERT_RIGHT:			funcName = "invert_r";			break;
		case UNARY_OP_INVERT_LEFT:			funcName = "invert_l";			break;
		case UNARY_OP_REVERSE:				funcName = "reverse";			break;
		case UNARY_OP_MAGNITUDE:			funcName = "mag";				break;
		case UNARY_OP_BAR:					funcName = "bar";				break;
		case BINARY_OP_SUM:					funcName = "sum/add";			break;
		case BINARY_OP_DIF:					funcName = "dif/sub";			break;
		case BINARY_OP_GP:					funcName = "gp";				break;
		case BINARY_OP_IP:					funcName = "ip";				break;
		case BINARY_OP_OP:					funcName = "op";				break;
		case BINARY_OP_COEF:				funcName = "coef";				break;
		case BINARY_OP_GET_GRADE_PART:		funcName = "get_grade_part";	break;
		case TURNARY_OP_SET_GRADE_PART:		funcName = "set_grade_part";	break;
	}

	// How many arguments does the operation take?
	int argCount = 0;
	switch( gaLuaOp )
	{
		case UNARY_OP_COPY:
		case UNARY_OP_NEGATE:
		case UNARY_OP_INVERT_LEFT:
		case UNARY_OP_INVERT_RIGHT:
		case UNARY_OP_REVERSE:
		case UNARY_OP_MAGNITUDE:
		case UNARY_OP_BAR:
		{
			argCount = 1;
			break;
		}
		case BINARY_OP_SUM:
		case BINARY_OP_DIF:
		case BINARY_OP_GP:
		case BINARY_OP_IP:
		case BINARY_OP_OP:
		case BINARY_OP_COEF:
		case BINARY_OP_GET_GRADE_PART:
		{
			argCount = 2;
			break;
		}
		case TURNARY_OP_SET_GRADE_PART:
		{
			argCount = 3;
			break;
		}
	}

	// Operations take one or more arguments.
	if( argCount <= 0 )
		GALuaError( L, "The function \"%s\" doesn't take any arguments, yet is expected to grab arguments.", funcName );

	// Make sure we were given at least the desired number of arguments.  We use the top stack values and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < argCount )
		GALuaError( L, "The function \"%s\" expects %d argument(s).", funcName, argCount );

	// Allocate room for our arguments and create a map of where they are on the Lua stack.
	GALuaUserData** argUserData = new GALuaUserData*[ argCount ];
	for( int i = 0; i < argCount; i++ )
		argUserData[i] = 0;

	// Try to go grab all of our arguments.
	int coercionCount = 0;
	for( int index = 0; index < argCount; index++ )
	{
		bool coercedUserData = false;
		int argIdx = -argCount + index - coercionCount;
		argUserData[ index ] = GrabGALuaUserData( L, argIdx, &coercedUserData );
		if( !argUserData[ index ] )
			GALuaError( L, "The function \"%s\" failed to grab argument %d.  Is it the right type?", funcName, index + 1 );

		// If coercion occurred, we must adjust our knowledge of the stack location of each argument relative to the top of the stack.
		// This is because the coerced user-data value is always pushed onto the top of the stack.
		if( coercedUserData )
			coercionCount++;
	}

	// Try to allocate memory for the result.
	GeometricAlgebra::SumOfBlades* opResult = new GeometricAlgebra::SumOfBlades();
	if( !opResult )
		GALuaError( L, "The function \"%s\" failed to allocate memory for the result.", funcName );

	// Try to perform the desired operation.
	bool operationPerformed = false;
	switch( gaLuaOp )
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
		case UNARY_OP_INVERT_LEFT:
		{
			GeometricAlgebra::SumOfBlades::InverseResult inverseResult;
			operationPerformed = opResult->AssignGeometricInverse( *argUserData[0]->multiVec, GeometricAlgebra::SumOfBlades::LEFT_INVERSE, inverseResult );
			break;
		}
		case UNARY_OP_INVERT_RIGHT:
		{
			GeometricAlgebra::SumOfBlades::InverseResult inverseResult;
			operationPerformed = opResult->AssignGeometricInverse( *argUserData[0]->multiVec, GeometricAlgebra::SumOfBlades::RIGHT_INVERSE, inverseResult );
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
		case UNARY_OP_BAR:
		{
			operationPerformed = opResult->AssignSumOfBlades( *argUserData[0]->multiVec );
			if( operationPerformed )
				operationPerformed = opResult->Bar();
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
		case BINARY_OP_COEF:
		{
			// This operation can be performed by the inner product, but it is provided mainly as an optimization (supposedly)
			// and also as a convenience or even something that might make the math easier to read, I guess.
			if( argUserData[1]->multiVec->BladeCount() != 1 )
				GALuaError( L, "The function \"coef\" expects its second argument to be a basis blade, not a blade in general and certainly not a non-homogeneous multivector." );
			GeometricAlgebra::Blade blade;
			operationPerformed = argUserData[1]->multiVec->AssignBladeTo( blade, 0 );
			if( operationPerformed )
			{
				GeometricAlgebra::Scalar scalar;
				operationPerformed = operationPerformed = argUserData[0]->multiVec->ScalarPart( blade, scalar );
				if( operationPerformed )
					operationPerformed = opResult->AssignScalar( scalar );
			}
			break;
		}
		case BINARY_OP_GET_GRADE_PART:
		{
			// The order of arguments here was chosen so that we can be compatible with the __index meta-method.
			double scalar;
			operationPerformed = argUserData[1]->multiVec->AssignScalarTo( scalar );
			if( operationPerformed )
			{
				int gradePart = int( scalar );
				operationPerformed = argUserData[0]->multiVec->GetGradePart( gradePart, *opResult );
			}
			break;
		}
		case TURNARY_OP_SET_GRADE_PART:
		{
			// The order of arguments here was chosen to that we can be compatible with the __newindex meta-method.
			double scalar;
			operationPerformed = argUserData[1]->multiVec->AssignScalarTo( scalar );
			if( operationPerformed )
			{
				int gradePart = int( scalar );
				operationPerformed = argUserData[0]->multiVec->SetGradePart( gradePart, *argUserData[2]->multiVec );
				if( operationPerformed )
					operationPerformed = opResult->AssignSumOfBlades( *argUserData[0]->multiVec );
			}
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

	// Called from Lua, there is no need to clean up the arguments we were
	// given on the stack before returning our result.  However, being called
	// internally, it is a good idea to adopt the convention of removing the
	// given arguments.  Everything is done relative to the top of the stack.
	// This allows other functions, that take input and give output to the stack,
	// to easily call this function, because we have a well defined behavior.
	for( int index = 0; index < argCount + coercionCount; index++ )
		lua_remove( L, -2 );

	// We return one argument at the top of the stack.
	return 1;
}

//=========================================================================================
int l_copy( lua_State* L )
{
	return PerformOp( L, UNARY_OP_COPY );
}

//=========================================================================================
int l_neg( lua_State* L )
{
	return PerformOp( L, UNARY_OP_NEGATE );
}

//=========================================================================================
int l_left_inverse( lua_State* L )
{
	return PerformOp( L, UNARY_OP_INVERT_LEFT );
}

//=========================================================================================
int l_right_inverse( lua_State* L )
{
	return PerformOp( L, UNARY_OP_INVERT_RIGHT );
}

//=========================================================================================
int l_reverse( lua_State* L )
{
	return PerformOp( L, UNARY_OP_REVERSE );
}

//=========================================================================================
int l_mag( lua_State* L )
{
	return PerformOp( L, UNARY_OP_MAGNITUDE );
}

//=========================================================================================
int l_bar( lua_State* L )
{
	return PerformOp( L, UNARY_OP_BAR );
}

//=========================================================================================
int l_sum( lua_State* L )
{
	return PerformOp( L, BINARY_OP_SUM );
}

//=========================================================================================
int l_dif( lua_State* L )
{
	return PerformOp( L, BINARY_OP_DIF );
}

//=========================================================================================
int l_gp( lua_State* L )
{
	return PerformOp( L, BINARY_OP_GP );
}

//=========================================================================================
int l_ip( lua_State* L )
{
	return PerformOp( L, BINARY_OP_IP );
}

//=========================================================================================
int l_op( lua_State* L )
{
	return PerformOp( L, BINARY_OP_OP );
}

//=========================================================================================
int l_coef( lua_State* L )
{
	return PerformOp( L, BINARY_OP_COEF );
}

//=========================================================================================
int l_get_grade_part( lua_State* L )
{
	return PerformOp( L, BINARY_OP_GET_GRADE_PART );
}

//=========================================================================================
int l_set_grade_part( lua_State* L )
{
	return PerformOp( L, TURNARY_OP_SET_GRADE_PART );
}

// Operation.cpp