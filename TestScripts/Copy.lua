-- Copy.lua

package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
package.cpath = package.cpath .. ";D:\\GALua\\Build\\Debug\\?.dll"

-- Acquire the GALua API.
local galua = require 'galua'

-- Configure our algebra.
local algebraFile = "../Algebras/EGA3D.lua"
local algebra = dofile( algebraFile )
if not algebra then
	error( "Failed to get algebra function from file \"" .. algebraFile .. "\"." )
end
algebra( galua )

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