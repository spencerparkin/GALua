-- Bar.lua

package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
package.cpath = package.cpath .. ";D:\\GALua\\Build\\Debug\\?.dll"

-- Acquire the GALua API.
local galua = require 'galua'

-- Configure our algebra.
local algebraFile = "../Algebras/QGA3D.lua"
local algebra = dofile( algebraFile )
if not algebra then
	error( "Failed to get algebra function from file \"" .. algebraFile .. "\"." )
end
algebra( galua )

local no = galua( "no" )
local ni = galua( "ni" )
local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

local nob = galua( "nob" )
local nib = galua( "nib" )
local e1b = galua( "e1b" )
local e2b = galua( "e2b" )
local e3b = galua( "e3b" )

local sphere_rad = 5
local sphere_pos = 3*e1 - 4*e2 + e3
local sphere = no + sphere_pos + 0.5*( sphere_pos*sphere_pos - sphere_rad*sphere_rad )*ni
print( "sphere = " .. tostring( sphere ) )

local sphere_bar = sphere:bar()
print( "sphere_bar = " .. tostring( sphere_bar ) )

local V = sphere*sphere_bar
print( "V = " .. tostring(V) )

-- Bar.lua