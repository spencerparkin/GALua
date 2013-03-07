-- Circle.lua

local galua = require 'galua'
local gautil = require 'gautil'
local cga = require 'cgautil'

gautil.CGA3D( galua )
cga.Setup( galua )

local point1 = cga.NewPoint{ center = cga.evec( -1, 0, 0 ) }:ComposeBlade()
local point2 = cga.NewPoint{ center = cga.evec( 1, 0, 0 ) }:ComposeBlade()
local point3 = cga.NewPoint{ center = cga.evec( 0, 1, 0 ) }:ComposeBlade()

local dirCircle = point1 ^ point2 ^ point3
local dualCircle = dirCircle * cga.I
local circleObj = cga.NewCircle( dualCircle )

for key, value in pairs( circleObj ) do
	print( key .. ": " .. tostring( value ) )
end

-- Circle.lua