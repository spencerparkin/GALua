// BinaryOp.cpp

#include "Lua.hpp"
#include "BinaryOp.h"
#include "UserData.h"
#include "Calculator/CalcLib.h"

//=========================================================================================
enum BinaryOp
{
	BINARY_OP_SUM,
	BINARY_OP_DIF,
	BINARY_OP_GP,
	BINARY_OP_IP,
	BINARY_OP_OP,
};

//=========================================================================================
static int PerformBinaryOp( lua_State* L, const char* funcName, BinaryOp binaryOp )
{
	char error[256];
	error[0] = '\0';
	GeometricAlgebra::SumOfBlades* opResult = 0;

	try
	{
		// Make sure we were given at least two arguments.  We use the top two stack values and ignore the rest.
		int stack_top = lua_gettop(L);
		if( stack_top < 2 )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" expects 2 arguments.", funcName );
			throw( error );
		}

		// Try to grab the first argument.
		bool coercedUserData = false;
		GALuaUserData* leftArgUserData = GrabGALuaUserData( L, -2, &coercedUserData );
		if( !leftArgUserData )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" failed to grab its first argument.  Is it the right type?", funcName );
			throw( error );
		}

		// Try to grab the second argument.
		int argumentIdx = coercedUserData ? -2 : -1;
		GALuaUserData* rightArgUserData = GrabGALuaUserData( L, argumentIdx, &coercedUserData );
		if( !rightArgUserData )
		{
			sprintf_s( error, sizeof( error ), "The function \"%s\" failed to grab its second argument.  Is it the right type?", funcName );
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
		switch( binaryOp )
		{
			case BINARY_OP_SUM:
			{
				operationPerformed = opResult->AssignSum( *leftArgUserData->multiVec, *rightArgUserData->multiVec );
				break;
			}
			case BINARY_OP_DIF:
			{
				operationPerformed = opResult->AssignDifference( *leftArgUserData->multiVec, *rightArgUserData->multiVec );
				break;
			}
			case BINARY_OP_GP:
			{
				operationPerformed = opResult->AssignGeometricProduct( *leftArgUserData->multiVec, *rightArgUserData->multiVec );
				break;
			}
			case BINARY_OP_IP:
			{
				operationPerformed = opResult->AssignInnerProduct( *leftArgUserData->multiVec, *rightArgUserData->multiVec );
				break;
			}
			case BINARY_OP_OP:
			{
				operationPerformed = opResult->AssignOuterProduct( *leftArgUserData->multiVec, *rightArgUserData->multiVec );
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

	// We return one argument.  This counts values starting from the top of the stack,
	// so the stack doesn't have to contain our returned value(s) exclusively.
	return 1;
}

//=========================================================================================
int l_sum( lua_State* L )
{
	return PerformBinaryOp( L, "sum", BINARY_OP_SUM );
}

//=========================================================================================
int l_dif( lua_State* L )
{
	return PerformBinaryOp( L, "dif", BINARY_OP_DIF );
}

//=========================================================================================
int l_gp( lua_State* L )
{
	return PerformBinaryOp( L, "gp", BINARY_OP_GP );
}

//=========================================================================================
int l_ip( lua_State* L )
{
	return PerformBinaryOp( L, "ip", BINARY_OP_IP );
}

//=========================================================================================
int l_op( lua_State* L )
{
	return PerformBinaryOp( L, "op", BINARY_OP_OP );
}

// BinaryOp.cpp