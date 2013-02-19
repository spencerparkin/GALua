-- CGAUtil.lua

local CGAUtil = {}

------------------------------------------------------------------------
local galua = nil

------------------------------------------------------------------------
local e1, e2, e3, no, ni
local no_ni
local Ie, I

------------------------------------------------------------------------
-- Before using the module, this function must be called to create
-- and initialize some constants that we use in the various API calls.
------------------------------------------------------------------------
function CGAUtil.MakeConstants( galua_api )

	-- Keep a reference to the GALua API table.
	galua = galua_api
	
	-- Create the basis vectors.
	e1 = galua( "e1" )
	e2 = galua( "e2" )
	e3 = galua( "e3" )
	no = galua( "no" )
	ni = galua( "ni" )
	
	-- Create a frequently recuring constant.
	no_ne = no ^ ni
	
	-- Create the Euclidean unit pseudo-scalar.
	Ie = e1 ^ e2 ^ e3
	
	-- Create the unit pseudo-scalar.
	I = Ie ^ no_ni
	
end

------------------------------------------------------------------------
-- Compose a dual or direct point.
------------------------------------------------------------------------
function CGAUtil.ComposePoint( pointInfo )
	local weight = poitnInfo.weight
	local center = pointInfo.center
	local point = weight * ( no + center + 0.5 * ( center .. center ) * ni )
	if not pointInfo.dual then
		point = point * I
	end
	return point
end

------------------------------------------------------------------------
-- Decompose a dual or direct point.
------------------------------------------------------------------------
function CGAUtil.DecomposePoint( point )
	local pointInfo = {}
	local dualPoint
	local dirPoint = point[4]
	if dirPoint ~= 0 then
		dualPoint = dirPoint * -I
		pointInfo.dual = false
	else
		dualPoint = point[1]
		pointInfo.dual = true
	end
	if dualPoint == 0 then
		return nil
	end
	pointInfo.weight = dualPoint .. -ni
	dualPoint = dualPoint / pointInfo.weight
	pointInfo.center = no_ni .. ( dualPoint ^ no_ni )
	return pointInfo
end

------------------------------------------------------------------------
-- Compose a dual or direct sphere.
------------------------------------------------------------------------
function CGAUtil.ComposeSphere( sphereInfo )
	local weight = sphereInfo.weight
	local center = sphereInfo.center
	local radius = sphereInfo.radius
	local sign = 1
	if sphereInfo.imaginary then
		sign = -1
	end
	local sphere = weight * ( no + center + 0.5 * ( center .. center - sign * radius * radius ) * ni )
	if not sphereInfo.dual then
		sphere = sphere * I
	end
	return sphere
end

------------------------------------------------------------------------
-- Decompose a dual or direct sphere.
------------------------------------------------------------------------
function CGAUtil.DecomposeSphere( sphere )
	return nil
end

------------------------------------------------------------------------
-- Compose a dual or direct plane.
------------------------------------------------------------------------
function CGAUtil.ComposePlane( planeInfo )
	local weight = planeInfo.weight
	local center = planeInfo.center
	local normal = planeInfo.normal
	local normal_len = #normal
	if normal_len ~= 1 then
		normal = normal / #normal
	end
	local plane = weight * ( normal + ( normal .. center ) * ni )
	if not planeInfo.dual then
		plane = plane * I
	end
	return plane
end

------------------------------------------------------------------------
-- Decompose a dual or direct plane.
------------------------------------------------------------------------
function CGAUtil.DecomposeSphere( plane )
	return nil
end

-- TODO: Create composition and decomposition methods for the rest of the CGA geometric primitives such as circles, lines, point-pairs, etc.

------------------------------------------------------------------------
-- Identify the given geometry.  Return a table with information
-- about what the given geometry is (e.g. a dual sphere, direct plane, etc.)
-- Included in the table will be the corresponding composition and
-- decomposition routines.  NIL is returned if we fail here to identify
-- the given blade as a geometric primitive of CGA.
------------------------------------------------------------------------
function CGAUtil.IdentifyGeometry( geo )
	return nil
end

-- Return the API table for this module.
return CGAUtil

-- CGAUtil.lua