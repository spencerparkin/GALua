-- Magnitude.lua

local galua = require 'galua'
local gautil = require 'gautil'

gautil.EGA3D( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

local E = e1 - e2^e1 + 3*e1^e2^e3
local E_mag = #E
print( "|" .. tostring(E) .. "| = " .. tostring(E_mag) )
local E_neg = -E
E_mag = #E_neg
print( "|" .. tostring(E_neg) .. "| = " .. tostring(E_mag) )

-- Magnitude.lua