-- Magnitude.lua

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

local E = e1 - e2^e1 + 3*e1^e2^e3
local E_mag = #E
print( "|" .. tostring(E) .. "| = " .. tostring(E_mag) )
local E_neg = -E
E_mag = #E_neg
print( "|" .. tostring(E_neg) .. "| = " .. tostring(E_mag) )

-- Magnitude.lua