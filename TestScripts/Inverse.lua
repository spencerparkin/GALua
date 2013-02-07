-- Inverse.lua

package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
package.cpath = package.cpath .. ";D:\\GALua\\Build\\Debug\\?.dll"

-- Acquire the GALua API.
local galua = require 'galua'

-- Configure our algebra.
local algebraFile = "../../Algebras/EGA3D.lua"
local algebra = dofile( algebraFile )
if not algebra then
	error( "Failed to algebra function from file \"" .. algebraFile .. "\"." )
end
algebra( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

local E = 3 + e1 + e1*e2
local E_inv_l = galua.invert_l(E)
local E_inv_r = galua.invert_r(E)
print( "E = " .. tostring(E) )
print( "E_inv_l = " .. tostring( E_inv_l ) )
print( "E_inv_r = " .. tostring( E_inv_r ) )
local E_l_check = E_inv_l * E
local E_r_check = E * E_inv_r
print( "E_l_check = " .. tostring( E_l_check ) )
print( "E_r_check = " .. tostring( E_r_check ) )

-- Inverse.lua