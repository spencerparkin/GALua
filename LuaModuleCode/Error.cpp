// Error.cpp

#include "lua.hpp"
#include "Error.h"
#include <string.h>

//=========================================================================================
void GALuaError( lua_State* L, const char* formatString, ... )
{
	// Formulate the error message.
	char errorMsg[2048];
	va_list args;
	va_start( args, formatString );
	vsprintf_s( errorMsg, sizeof( errorMsg ), formatString, args );
	va_end( args );

	/// Ha!  There is no need to do this.  luaL_error() already does this for us.
#if 0
	// Try to tack on a trace-back message.
	lua_getglobal( L, "debug" );
	if( !lua_isnil( L, -1 ) )
	{
		lua_getfield( L, -1, "traceback" );
		if( !lua_isnil( L, -1 ) )
		{
			if( 0 == lua_pcall( L, 0, 1, 0 ) )
			{
				const char* tracebackMsg = lua_tostring( L, -1 );
				if( tracebackMsg )
				{
					strcat_s( errorMsg, sizeof( errorMsg ), "\n\n" );
					strcat_s( errorMsg, sizeof( errorMsg ), tracebackMsg );
				}
			}
		}
	}
#endif

	// Issue a Lua error.  This is fatal for the currently
	// running chunk in that we cannot recover-from the
	// error and continue running.  This will jump us to
	// the return address of the call to our chunk, I think.
	luaL_error( L, errorMsg );
}

// Error.cpp