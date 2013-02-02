// UnaryOp.cpp

#include "lua.hpp"
#include "UnaryOp.h"
#include "UserData.h"
#include "Calculator/CalcLib.h"

//=========================================================================================
enum UnaryOp
{
	UNARY_OP_COPY,
	UNARY_OP_NEGATE,
	UNARY_OP_INVERT,
	UNARY_OP_REVERSE,
	UNARY_OP_MAGNITUDE,
};

//=========================================================================================
int PerformUnaryOp( lua_State* L, const char* funcName, UnaryOp unaryOp )
{
	char error[2048];
	error[0] = '\0';
	GeometricAlgebra::SumOfBlades* opResult = 0;

	try
	{
		// Make sure we were given exactly one argument.
		int stack_top = lua_gettop(L);
		if( stack_top != 1 && /* Hack: Unary overloads get 2 arguments? */ stack_top != 2 )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" expects 1 and only 1 argument.", funcName );
			throw( error );
		}

		// Try to grab our argument.
		GALuaUserData* argUserData = GrabGALuaUserData( L, -1 );
		if( !argUserData )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" failed to grab its one and only argument.", funcName );
			throw( error );
		}

		// Try to allocate memory for the result.
		opResult = new GeometricAlgebra::SumOfBlades();
		if( !opResult )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" failed to allocate memory for the result.", funcName );
			throw( error );
		}

		// Try to perform the desired operation.
		bool operationPerformed = false;
		switch( unaryOp )
		{
			case UNARY_OP_COPY:
			{
				operationPerformed = opResult->AssignSumOfBlades( *argUserData->multiVec );
				break;
			}
			case UNARY_OP_NEGATE:
			{
				operationPerformed = opResult->AssignSumOfBlades( *argUserData->multiVec );
				if( operationPerformed )
					operationPerformed = opResult->Scale( GeometricAlgebra::Scalar( -1.0 ) );
				break;
			}
			case UNARY_OP_INVERT:
			{
				operationPerformed = false;

				// TODO: This algorithm needs to be revisited.
				//operationPerformed = opResult->AssignGeometricInverse( *argUserData->multiVec );
				break;
			}
			case UNARY_OP_REVERSE:
			{
				operationPerformed = opResult->AssignSumOfBlades( *argUserData->multiVec );
				if( operationPerformed )
					operationPerformed = opResult->Reverse();
				break;
			}
			case UNARY_OP_MAGNITUDE:
			{
				GeometricAlgebra::Scalar squareMagnitude;
				operationPerformed = argUserData->multiVec->AssignSquareMagnitudeTo( squareMagnitude );
				if( operationPerformed )
				{
					double magnitude = sqrt( squareMagnitude );
					operationPerformed = opResult->AssignScalarFrom( magnitude );
				}
				break;
			}
		}

		// Did it work?!
		if( !operationPerformed )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" failed to perform the desired operation.", funcName );
			throw( error );
		}

		// Create some user data for the result.
		GALuaUserData* resultUserData = NewGALuaUserData(L);
		if( !resultUserData )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" failed to create Lua user-data for the result.", funcName );
			throw( error );
		}

		// Save off the pointer in Lua.
		resultUserData->multiVec = opResult;
	}
	catch( const char* )
	{
	}

	// If something went wrong, tell Lua.
	if( error[0] != '\0' )
	{
		// If a failure occured, Lua should not have a pointer to the memory
		// we allocated, so it should be safe to delete it here.
		delete opResult;
		luaL_error( L, error );
	}

	return 1;
}

//=========================================================================================
int l_copy( lua_State* L )
{
	return PerformUnaryOp( L, "copy", UNARY_OP_COPY );
}

//=========================================================================================
int l_negate( lua_State* L )
{
	return PerformUnaryOp( L, "negate", UNARY_OP_NEGATE );
}

//=========================================================================================
int l_invert( lua_State* L )
{
	return PerformUnaryOp( L, "invert", UNARY_OP_INVERT );
}

//=========================================================================================
int l_reverse( lua_State* L )
{
	return PerformUnaryOp( L, "reverse", UNARY_OP_REVERSE );
}

//=========================================================================================
int l_mag( lua_State* L )
{
	return PerformUnaryOp( L, "mag", UNARY_OP_MAGNITUDE );
}

// UnaryOp.cpp