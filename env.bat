@echo OFF

rem Make sure we can find the interpreter.  Note that we prepend our path so that we don't find
rem an earlier version of the interpreter on the system.  Our module is not compatible with Lua 5.1.
set PATH=C:/GALua/lua-5.2.1/bin;D:/GALua/lua-5.2.1/bin;%PATH%

rem Set the LUA_CPATH env-var so that Lua can find our Lua modules written in C++.
set GALUA_CPATH=C:/GALua/Build/?.debug.dll;D:/GALua/Build/?.debug.dll
set GALUA_CPATH=%GALUA_CPATH%;C:/GALua/Build/?.release.dll;D:/GALua/Build/?.release.dll
if "%LUA_CPATH%"=="" (
	set LUA_CPATH=%GALUA_CPATH%;;
) else (
	set LUA_CPATH=%LUA_CPATH%;%GALUA_CPATH%
)

rem Set the LUA_PATH env-var so that Lua can find our Lua modules written in Lua.
set GALUA_PATH=C:/GALua/SupplementaryModules/?.lua;D:/GALua/SupplementaryModules/?.lua
if "%LUA_PATH%"=="" (
	set LUA_PATH=%GALUA_PATH%;;
) else (
	set LUA_PATH=%LUA_PATH%;%GALUA_PATH%
)

rem We're done!
echo Environment variables set!