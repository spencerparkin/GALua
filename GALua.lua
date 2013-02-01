-- Test the GALua module.

--package.cpath = package.cpath .. ";.\\Debug\\?.dll"
package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
print( package.cpath )

require 'galua'

--[[
local blah = "blah"
local bloop = galua.to_number( blah )
local bloopy = galua.to_string( bloop )

print( "bloop = " .. bloop )
print( "bloopy = " .. bloopy )
]]