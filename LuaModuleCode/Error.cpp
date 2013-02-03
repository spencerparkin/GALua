// Error.cpp

#include "lua.hpp"
#include "Error.h"

//=========================================================================================
void GALuaError( lua_State* L, const char* formatString, ... )
{
	char errorMsg[2048];
	va_list args;
	va_start( args, formatString );
	vsprintf_s( errorMsg, sizeof( errorMsg ), formatString, args );
	va_end( args );

	luaL_error( L, errorMsg );
}

// Error.cpp