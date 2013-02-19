-- Copy.lua

local galua = require 'galua'
local gautil = require 'gautil'

gautil.EGA3D( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

-- Note that the grade-setting part modifies both multi-vectors.
local multiVecA = 2*e1
local multiVecB = multiVecA
multiVecB[2] = e1^e2
print( "multiVecA = " .. tostring( multiVecA ) )
print( "multiVecB = " .. tostring( multiVecB ) )

-- Now the grade-setting part modifies an independent copy.
local multiVecA = 2*e1
local multiVecB = multiVecA:copy()
multiVecB[2] = e1^e2
print( "multiVecA = " .. tostring( multiVecA ) )
print( "multiVecB = " .. tostring( multiVecB ) )

-- Copy.lua