// Debug.cpp

#include "lua.hpp"
#include "Debug.h"

//=========================================================================================
static const char* TypeString( int type )
{
	switch( type )
	{
		case LUA_TNONE:					return "TNONE";
		case LUA_TNIL:					return "TNIL";
		case LUA_TBOOLEAN:				return "TBOOLEAN";
		case LUA_TLIGHTUSERDATA:		return "TLIGHTUSERDATA";
		case LUA_TNUMBER:				return "TNUMBER";
		case LUA_TSTRING:				return "TSTRING";
		case LUA_TTABLE:				return "TTABLE";
		case LUA_TFUNCTION:				return "TFUNCTION";
		case LUA_TUSERDATA:				return "TUSERDATA";
		case LUA_TTHREAD:				return "TTHREAD";
	}
	return "???";
}

//=========================================================================================
static void StackEntryString( lua_State* L, int idx, char* stackEntryStr, int stackEntryStrSize )
{
	int type = lua_type( L, idx );
	const char* typeStr = TypeString( type );

	switch( type )
	{
		case LUA_TNONE:
		case LUA_TNIL:
		case LUA_TBOOLEAN:
		case LUA_TNUMBER:
		case LUA_TSTRING:
		{
			const char* valueStr = lua_tostring( L, idx );
			sprintf_s( stackEntryStr, stackEntryStrSize, "%d -> %s -> %s", idx, typeStr, valueStr );
			break;
		}
		case LUA_TTABLE:
		case LUA_TLIGHTUSERDATA:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
		{
			sprintf_s( stackEntryStr, stackEntryStrSize, "%d -> %s", idx, typeStr );
			break;
		}
		default:
		{
			stackEntryStr[0] = '?';
			stackEntryStr[1] = '\0';
			break;
		}
	}
}

//=========================================================================================
void DumpStack( lua_State* L )
{
	printf( "Stack Dump...\n{\n" );
	int stack_top = lua_gettop(L);
	for( int idx = 1; idx <= stack_top; idx++ )
	{
		char stackEntryStr[128];
		StackEntryString( L, idx, stackEntryStr, sizeof( stackEntryStr ) );
		printf( "\t%s\n", stackEntryStr );
	}
	printf( "}\n" );
}

// Debug.cpp