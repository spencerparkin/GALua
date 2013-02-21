-- TestCGAUtil.lua

local galua = require 'galua'
local gautil = require 'gautil'
local cga = require 'cgautil'

gautil.CGA3D( galua )
cga.Setup( galua )

local pointA = cga.NewPoint{ center = cga.evec( -1, 2, -1 ) }:ComposeBlade()
local pointB = cga.NewPoint{ center = cga.evec( 1, 2, 3 ) }:ComposeBlade()
local pointC = cga.NewPoint{ center = cga.evec( -2, 2, 1 ) }:ComposeBlade()
local pointD = cga.NewPoint{ center = cga.evec( 1, -3, 4 ) }:ComposeBlade()

print( "A: " .. tostring( pointA ) )
print( "B: " .. tostring( pointB ) )
print( "C: " .. tostring( pointC ) )
print( "D: " .. tostring( pointD ) )

local blade = pointA ^ pointB ^ pointC ^ pointD
print( tostring( blade ) )

local sphere = cga.NewSphere{}
if sphere:DecomposeBlade( blade ) then
	for key, value in pairs( sphere ) do
		print( key .. ": " .. tostring( value ) )
	end
else
	print( "Decomposition failed.  It's a direct plane!  :)" )
end

-- TestCGAUtil.lua