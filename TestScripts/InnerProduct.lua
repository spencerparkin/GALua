-- InnerProduct.lua

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

vecA = e1 + e2 + e3
vecB = e1 - e2 + 2*e3
dot = vecA[ vecB ]
print( "dot = " .. tostring( dot ) )

vecA = e1 + e2 + e3
vecB = e1 - e2 + 2*e3
dot = vecA . vecB		-- Now it passes "vecB", the string.
print( "dot = " .. tostring( dot ) )

-- InnerProduct.lua