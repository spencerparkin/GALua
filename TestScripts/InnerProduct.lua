-- InnerProduct.lua

local galua = require 'galua'
local gautil = require 'gautil'

gautil.EGA3D( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

vecA = e1 + e2 + e3
vecB = e1 - e2 + 2*e3
dot = vecA[ vecB ]
print( "dot = " .. tostring( dot ) )

vecA = e1 + e2 + e3
vecB = e1 - e2 + 2*e3
dot = vecA . vecB		-- Now it passes "vecB", the string.
print( "dot = " .. tostring( dot ) )

-- InnerProduct.lua