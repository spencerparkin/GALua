-- Test.lua

local galua_api = require 'galua'
local gautil = require 'gautil'

gautil.EGA3D( galua_api )

local e1 = galua_api( "e1" )
local e2 = galua_api( "e2" )
local e3 = galua_api( "e3" )

local vecA = e1 + e2
local vecB = e2 + e3
local rotor = vecA * vecB
print( "rotor = " .. tostring( rotor ) )
print( "rotor[0] = " .. tostring( rotor[0] ) )
print( "rotor[1] = " .. tostring( rotor[1] ) )
print( "rotor[2] = " .. tostring( rotor[2] ) )

-- Test.lua