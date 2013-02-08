// String.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

//=========================================================================================
#include "lua.hpp"
#include "String.h"
#include "UserData.h"
#include "Error.h"
#include "Environment.h"
#include "Calculator/CalcLib.h"

//=========================================================================================
int l_from_string( lua_State* L )
{
	CalcLib::Evaluator* evaluator = 0;
	CalcLib::Number* numberResult = 0;
	GeometricAlgebra::SumOfBlades* opResult = 0;

	// Make sure we were given an argument.  We will only use the top value in the stack and ignore the rest.
	int stack_top = lua_gettop(L);
	if( stack_top < 1 )
		GALuaError( L, "The function \"from_string\" expects an argument." );

	// Make sure our argument is a string.
	if( !lua_isstring( L, -1 ) )
		GALuaError( L, "The function \"from_string\" expects its argument to be a string." );

	// Try to compile the given expression.
	const char* expression = lua_tostring( L, -1 );
	CalcLib::Calculator calculator( gaLuaEnv, false );
	evaluator = calculator.CompileEvaluator( expression );
	if( !evaluator )
	{
		char printBuffer[2048];
		gaLuaEnv->PrintBuffer( printBuffer, sizeof( printBuffer ) );
		gaLuaEnv->PrintAllErrors();
		GALuaError( L, "The function \"from_string\" failed to compiled the given expression \"%s\".\n\n%s\n", expression, printBuffer );
	}

	// Try to create a number for the result.  That we have to do this cast is fairly annoying.
	CalcLib::Environment* env = gaLuaEnv->Cast< CalcLib::Environment >();
	numberResult = env->CreateNumber( evaluator );
	if( !numberResult )
		GALuaError( L, "The function \"from_string\" failed to allocate memory for the result of the given expression." );

	// Try to evaluate the result.
	if( !evaluator->EvaluateResult( *numberResult, *gaLuaEnv ) )
		GALuaError( L, "The function \"from_string\" failed to evaluate the expression \"%s\".", expression );

	// Try to create a multivector for the result.
	opResult = new GeometricAlgebra::SumOfBlades();
	if( !opResult )
		GALuaError( L, "The function \"from_string\" failed to allocate memory for the result of the given expression." );

	// Try to create some user-data for the result.
	GALuaUserData* resultUserData = NewGALuaUserData(L);
	if( !resultUserData )
		GALuaError( L, "The function \"from_string\" failed to create user-data for the result of the given expression." );

	// Assign the result and save off the pointer to Lua.
	CalcLib::MultivectorNumber* multiVecNumber = numberResult->Cast< CalcLib::MultivectorNumber >();
	multiVecNumber->AssignTo( *opResult, *gaLuaEnv );
	resultUserData->multiVec = opResult;

	// Clean up our mess, if any.
	delete evaluator;
	delete numberResult;

	// Return one result to the caller.
	return 1;
}

//=========================================================================================
// For this to provide the inverse function of "from_string", the CalcLib library
// must be able to print multi-vectors in a parsable format.  This is something
// I should revisit in CalcLib, and even provide explicit support for.
int l_to_string( lua_State* L )
{
	// Make sure we were given exactly one argument.
	int stack_top = lua_gettop(L);
	if( stack_top != 1 )
		GALuaError( L, "The function \"to_string\" expects 1 and only 1 argument." );

	// Try to grab our argument.
	GALuaUserData* userData = GrabGALuaUserData( L, -1 );
	if( !userData )
		GALuaError( L, "The function \"to_string\" failed to grab its one and only argument." );

	// Print the argument to a buffer in a way that it can be read back in using "from_string".
	// TODO: There should be a "PRINT_FOR_PARSING" here so that we can insure we're an inverse for "from_string".
	char printBuffer[1024];
	if( !userData->multiVec->Print( printBuffer, sizeof( printBuffer ), ScalarAlgebra::PRINT_FOR_READING ) )
		GALuaError( L, "The function \"to_string\" failed to print the given multivector." );

	// Push the string to return it to the caller.
	lua_pushstring( L, printBuffer );
	
	// We return one value.
	return 1;
}

// String.cpp