// String.cpp

//=========================================================================================
#include "lua.hpp"
#include "String.h"
#include "Calculator/CalcLib.h"
#include "UserData.h"
#include "Env.h"

//=========================================================================================
int l_from_string( lua_State* L )
{
	char error[2048];
	error[0] = '\0';
	CalcLib::Evaluator* evaluator = 0;
	CalcLib::Number* numberResult = 0;
	GeometricAlgebra::SumOfBlades* opResult = 0;

	try
	{
		// Make sure we were given exactly one argument.
		int stack_top = lua_gettop(L);
		if( stack_top != 1 )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" expects 1 and only 1 argument." );
			throw;
		}

		// Make sure our argument is a string.
		if( !lua_isstring( L, -1 ) )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" expects its one and only argument to be a string." );
			throw;
		}

		// Try to compile the given expression.
		const char* expression = lua_tostring( L, -1 );
		CalcLib::Calculator calculator( gaLuaEnv, false );
		evaluator = calculator.CompileEvaluator( expression );
		if( !evaluator )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" failed to compiled the given expression \"%s\".\n\n", expression );
			char printBuffer[2048];
			gaLuaEnv->PrintBuffer( printBuffer, sizeof( printBuffer ) );
			gaLuaEnv->PrintAllErrors();
			strcat_s( error, sizeof( error ), printBuffer );
			throw;
		}

		// Try to create a number for the result.  That we have to do this cast is fairly annoying.
		CalcLib::Environment* env = gaLuaEnv->Cast< CalcLib::Environment >();
		numberResult = env->CreateNumber( evaluator );
		if( !numberResult )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" failed to allocate memory for the result of the given expression." );
			throw;
		}

		// Try to evaluate the result.
		if( !evaluator->EvaluateResult( *numberResult, *gaLuaEnv ) )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" failed to evaluate the expression \"%s\".", expression );
			throw;
		}

		// Try to create a multivector for the result.
		opResult = new GeometricAlgebra::SumOfBlades();
		if( !opResult )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" failed to allocate memory for the result of the given expression." );
			throw;
		}

		// Try to create some user-data for the result.
		GALuaUserData* resultUserData = NewGALuaUserData(L);
		if( !resultUserData )
		{
			sprintf_s( error, sizeof( error ), "The function \"from_string\" failed to create user-data for the result of the given expression." );
			throw;
		}

		// Assign the result and save off the pointer to Lua.
		CalcLib::MultivectorNumber* multiVecNumber = numberResult->Cast< CalcLib::MultivectorNumber >();
		multiVecNumber->AssignTo( *opResult, *gaLuaEnv );
		resultUserData->multiVec = opResult;
	}
	catch(...)
	{
	}

	// Clean up our mess, if any.
	delete evaluator;
	delete numberResult;

	// If something went wrong, tell Lua.
	if( error[0] != '\0' )
	{
		// We assume that if any error occurred, Lua does not have a pointer to this memory,
		// so it should be safe for us to delete it here.
		delete opResult;
		luaL_error( L, error );
	}

	// Return one result to the caller.
	return 1;
}

//=========================================================================================
// For this to provide the inverse function of "from_string", the CalcLib library
// must be able to print multi-vectors in a parsable format.  This is something
// I should revisit in CalcLib, and even provide explicit support for.
int l_to_string( lua_State* L )
{
	char error[2048];
	error[0] = '\0';
	
	try
	{
		// Make sure we were given exactly one argument.
		int stack_top = lua_gettop(L);
		if( stack_top != 1 )
		{
			sprintf_s( error, sizeof( error ), "The function \"to_string\" expects 1 and only 1 argument." );
			throw;
		}

		// Try to grab our argument.
		GALuaUserData* userData = GrabGALuaUserData( L, -1 );
		if( !userData )
		{
			sprintf_s( error, sizeof( error ), "The function \"to_string\" failed to grab its one and only argument." );
			throw;
		}

		// Print the argument to a buffer in a way that it can be read back in using "from_string".
		// TODO: There should be a "PRINT_FOR_PARSING" here so that we can insure we're an inverse for "from_string".
		char printBuffer[1024];
		if( !userData->multiVec->Print( printBuffer, sizeof( printBuffer ), ScalarAlgebra::PRINT_FOR_READING ) )
		{
			sprintf_s( error, sizeof( error ), "The function \"to_string\" failed to print the given multivector." );
			throw;
		}

		// Push the string to return it to the caller.
		lua_pushstring( L, printBuffer );
	}
	catch(...)
	{
	}

	// If something went wrong, tell Lua.
	if( error[0] != '\0' )
		luaL_error( L, error );
	
	// We return one value.
	return 1;
}

// String.cpp