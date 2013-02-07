-- GradePart.lua

package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
package.cpath = package.cpath .. ";D:\\GALua\\Build\\Debug\\?.dll"

-- Acquire the GALua API.
local galua = require 'galua'

-- Configure our algebra.
local algebraFile = "../Algebras/EGA3D.lua"
local algebra = dofile( algebraFile )
if not algebra then
	error( "Failed to algebra function from file \"" .. algebraFile .. "\"." )
end
algebra( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

local E = -5 + e1 - 2*e2 + e1^e2 - 3*e3^e2 + 3*e1^e2^e3
print( "E = " .. tostring(E) )
local E0 = galua.get_grade_part( E, 0 )
local E1 = galua.get_grade_part( E, 1 )
local E2 = galua.get_grade_part( E, 2 )
local E3 = galua.get_grade_part( E, 3 )
print( "E0 = " .. tostring(E0) )
print( "E1 = " .. tostring(E1) )
print( "E2 = " .. tostring(E2) )
print( "E3 = " .. tostring(E3) )
local E0 = E[0]
local E1 = E[1]
local E2 = E[2]
local E3 = E[3]
print( "Now using __index, we get..." )
print( "E0 = " .. tostring(E0) )
print( "E1 = " .. tostring(E1) )
print( "E2 = " .. tostring(E2) )
print( "E3 = " .. tostring(E3) )

-- GradePart.lua