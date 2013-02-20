-- CGAUtil.lua

------------------------------------------------------------------------
-- This will be the API table for this module.
local CGAUtil = {}

-- TODO: A better way to do this module would be to use OOP, and
--       make all the CGA geometric primitives objects with methods.
--       These objects could be manipulated by tweaking their parameters,
--       or by decomposing a blade into them.  They will also support
--       the composition of a blade representative of them.

------------------------------------------------------------------------
-- The set of all geometric primitives supported by CGA can
-- be divided into two catagories: those that are round and
-- those that are flat.
CGAUtil.round = {}
CGAUtil.flat = {}

------------------------------------------------------------------------
-- Here are the round geometries in order of increasing dimension.
-- The degenerate rounds will serve as the tangent points.
-- Rounds can be real or imaginary.
CGAUtil.round.point = { dimension = 0, dualGrade = 4, dirGrade = 1, dualIsDirect = true }
CGAUtil.round.pointpair = { dimension = 1, dualGrade = 3, dirGrade = 2, dualIsDirect = false }
CGAUtil.round.circle = { dimension = 2, dualGrade = 2, dirGrade = 3, dualIsDirect = false }
CGAUtil.round.sphere = { dimension = 3, dualGrade = 1, dirGrade = 4, dualIsDirect = false }

------------------------------------------------------------------------
-- Here are the flat geometries in order of increasing dimension.
-- The flats at infinity will serve as the free blades.
CGAUtil.flat.point = { dimension = 0, dualGrade = 3, dirGrade = 2, dualIsDirect = false }
CGAUtil.flat.line = { dimension = 1, dualGrade = 2, dirGrade = 3, dualIsDirect = false }
CGAUtil.flat.plane = { dimension = 2, dualGrade = 1, dirGrade = 4, dualIsDirect = false }

------------------------------------------------------------------------
-- Keep our own reference to the GALua module's API table.
local galua = nil

------------------------------------------------------------------------
local e1, e2, e3, no, ni
local no_ni
local Ie, I

------------------------------------------------------------------------
-- Before using the module, this function must be called to create
-- and initialize some constants that we use in the various API calls.
------------------------------------------------------------------------
function CGAUtil.Setup( galua_api )

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
-- Provide a convenient function that helps you create Euclidean vectors.
-- These often represent pure directions with magnitude or positions in space.
------------------------------------------------------------------------
function CGAUtil.EVec( x, y, z )
	local eVec = x*e1 + y*e2 + z*e3
	return eVec
end

------------------------------------------------------------------------
-- Compose a dual or direct point.
-- A null vector may be thought of as a dual or direct point.
-- Likewise, the corresponding pseudo-vectors may be thought
-- of as dual or direct.  Here we are arbitrarily choosing to
-- refer to null vectors as dual points and the corresponding
-- pseudo-vectors as direct points.  We'll do this in the
-- corresponding decomposition routine too.
------------------------------------------------------------------------
function CGAUtil.round.point.Compose( pointInfo )

	-- Gather the point parameters.
	local weight = pointInfo.weight and pointInfo.weight or 1
	local center = pointInfo.center
	
	-- Make a dual point.
	local point = weight * ( no + center + 0.5 * ( center .. center ) * ni )
	
	-- Make it a direct point if parameterized to do so.
	if not pointInfo.dual then
		point = point * I
	end
	
	-- Return the point!
	return point
	
end

------------------------------------------------------------------------
-- Decompose a dual or direct point.
------------------------------------------------------------------------
function CGAUtil.round.point.Decompose( point )

	-- Start the info table that we'll return.
	local pointInfo = {}
	
	-- We'll decompose a dual point, but were we given a dual or direct point?
	local dualPoint
	local dirPoint = point[4]
	if dirPoint ~= 0 then
		dualPoint = dirPoint * -I
		pointInfo.dual = false
	else
		dualPoint = point[1]
		pointInfo.dual = true
	end
	
	-- If we were given neither, then the given element is not a CGA point.
	if dualPoint == 0 then
		return nil
	end
	
	-- Decompose the point!
	pointInfo.weight = dualPoint .. -ni
	dualPoint = dualPoint / pointInfo.weight
	pointInfo.center = no_ni .. ( dualPoint ^ no_ni )
	
	-- Return the info table!
	return pointInfo
	
end

------------------------------------------------------------------------
-- Compose a dual or direct point-pair.
------------------------------------------------------------------------
function CGAUtil.round.pointpair.Compose( pointPairInfo )
	return nil
end

------------------------------------------------------------------------
-- Decompose a dual or direct point-pair.
------------------------------------------------------------------------
function CGAUtil.round.pointpair.Decompose( pointPair )
	return nil
end

------------------------------------------------------------------------
-- Compose a dual or direct circle.
------------------------------------------------------------------------
function CGAUtil.round.circle.Compose( circleInfo )
	return nil
end

------------------------------------------------------------------------
-- Decompose a dual or direct circle.
------------------------------------------------------------------------
function CGAUtil.round.circle.Decompose( circle )
	return nil
end

------------------------------------------------------------------------
-- Compose a dual or direct sphere.
------------------------------------------------------------------------
function CGAUtil.round.sphere.Compose( sphereInfo )

	-- Gather the sphere parameters.
	local weight = sphereInfo.weight and sphereInfo.weight or 1
	local center = sphereInfo.center
	local radius = sphereInfo.radius
	local imaginary = sphereInfo.imaginary and sphereInfo.imaginary or false
	local dual = sphereInfo.dual and sphereInfo.dual or true
	
	-- Are we dealing with a real or imaginary sphere?
	if radius < 0 then
		radius = -radius
		sphereInfo.imaginary = not sphereInfo.imaginary
	end
	local sign = 1
	if sphereInfo.imaginary then
		sign = -1
	end
	
	-- Make a dual sphere.
	local sphere = weight * ( no + center + 0.5 * ( center .. center - sign * radius * radius ) * ni )
	
	-- Make it a direct sphere if parameterized to do so.
	if not sphereInfo.dual then
		sphere = sphere * I
	end
	
	-- Return the sphere!
	return sphere
	
end

------------------------------------------------------------------------
-- Decompose a dual or direct sphere.
------------------------------------------------------------------------
function CGAUtil.round.sphere.Decompose( sphere )
	
	-- Start the info table that we'll return.
	local sphereInfo = {}
	
	-- We'll decompose a dual sphere, but were we given a dual or direct sphere?
	local dualSphere
	local dirSphere = sphere[4]
	if dirSphere ~= 0 then
		dualSphere = dirSphere * -I
		sphereInfo.dual = false
	else
		dualSphere = sphere[1]
		sphereInfo.dual = true
	end
	
	-- If we were given neither, then the given element is not a CGA sphere.
	if dualSphere == 0 then
		return nil
	end
	
	-- Decompose the sphere!
	sphereInfo.weight = dualSphere .. -ni
	dualSphere = dualSphere / sphereInfo.weight
	sphereInfo.center = no_ni .. ( dualPoint ^ no_ni )
	local radicand = ( 2 * dualSphere .. no + sphereInfo.center * sphereInfo.center ):to_number()
	sphereInfo.imaginary = false
	if radicand < 0 then
		sphereInfo.imaginary = true
		radicand = -radicand
	end
	sphereInfo.radius = math.sqrt( radicand )
	
	-- Return the info table!
	return sphereInfo
	
end

------------------------------------------------------------------------
-- Compose a dual or direct flat point.
------------------------------------------------------------------------
function CGAUtil.flat.point.Compose( flatPointInfo )
	return nil
end

------------------------------------------------------------------------
-- Decompose a dual or direct flat point.
------------------------------------------------------------------------
function CGAUtil.flat.point.Decompose( flatPoint )
	return nil
end

------------------------------------------------------------------------
-- Compose a dual or direct line.
------------------------------------------------------------------------
function CGAUtil.flat.line.Compose( lineInfo )
	return nil
end

------------------------------------------------------------------------
-- Decompose a dual or direct line.
------------------------------------------------------------------------
function CGAUtil.flat.line.Decompose( line )
	return nil
end

------------------------------------------------------------------------
-- Compose a dual or direct plane.
------------------------------------------------------------------------
function CGAUtil.flat.plane.Compose( planeInfo )

	-- Gather the plane parameters.
	local weight = planeInfo.weight
	local center = planeInfo.center
	local normal = planeInfo.normal
	
	-- Make sure the plane normal is a unit-length vector.
	local normal_len = #normal
	if normal_len ~= 1 then
		normal = normal / normal_len
	end
	
	-- Make a dual plane.
	local plane = weight * ( normal + ( normal .. center ) * ni )
	
	-- Make it a direct plane if parameterized to do so.
	if not planeInfo.dual then
		plane = plane * I
	end
	
	-- Return the plane!
	return plane
	
end

------------------------------------------------------------------------
-- Decompose a dual or direct plane.
------------------------------------------------------------------------
function CGAUtil.flat.plane.Decompose( plane )

	-- Start the info table that we'll return.
	local planeInfo = {}
	
	-- We'll decompose a dual plane, but were we given a dual or direct plane?
	local dualPlane
	local dirPlane = plane[4]
	if dualPlane ~= 0 then
		dualSphere = dualPlane * -I
		planeInfo.dual = false
	else
		dualPlane = plane[1]
		planeInfo.dual = true
	end
	
	-- Return the info table!
	return planeInfo

end

------------------------------------------------------------------------
-- Identify the given geometry.  Return the API sub-table containing
-- information about the identified geometry and methods that operate
-- on such types of geometries.  NIL is returned if here we fail to
-- identify the given element as a blade representative of a CGA geometry.
------------------------------------------------------------------------
function CGAUtil.IdentifyGeometry( geo )
	return nil
end

-- Return the API table for this module.
return CGAUtil

-- CGAUtil.lua