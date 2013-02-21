-- CGAUtil.lua

------------------------------------------------------------------------
-- This will be the API table for this module.
local CGAUtil = {}

------------------------------------------------------------------------
local galua = nil

------------------------------------------------------------------------
local e1, e2, e3, no, ni
local Ie, no_ni, I

------------------------------------------------------------------------
function CGAUtil.Setup( galua_api )
	
	-- Keep our own reference around to the GALua API table.
	galua = galua_api
	
	-- Create a frequently recuring constant.
	e1 = galua( "e1" )
	e2 = galua( "e2" )
	e3 = galua( "e3" )
	no = galua( "no" )
	ni = galua( "ni" )
	no_ni = no ^ ni
	Ie = e1 ^ e2 ^ e3
	I = Ie ^ no_ni
	
end

------------------------------------------------------------------------
-- Provide a convenience routine for creating Euclidean vectors.
function CGAUtil.evec( x, y, z )
	return x * e1 + y * e2 + z * e3
end

------------------------------------------------------------------------
-- Create a base class for all CGA geometries.
local CGAGeometry = {}

------------------------------------------------------------------------
function CGAGeometry:Setup()
	self.epsilon = 0.001
end

------------------------------------------------------------------------
function CGAGeometry:New( geo )
	local geo = geo or {}
	setmetatable( geo, self )
	self.__index = self
	geo:Setup()
	return geo
end

------------------------------------------------------------------------
-- Create the CGA geometric primitives as derivatives of our CGA geometry base class.
local CGAPoint = CGAGeometry:New()
local CGAFlatPoint = CGAGeometry:New()
local CGAPointPair = CGAGeometry:New()
local CGALine = CGAGeometry:New()
local CGACircle = CGAGeometry:New()
local CGAPlane = CGAGeometry:New()
local CGASphere = CGAGeometry:New()

------------------------------------------------------------------------
function CGAUtil.IdentifyBlade( blade )
	-- TODO: Return here the class table that can decompose the given blade?
	--       No maybe just an instance of an object that is its decomposition.
	return nil
end

------------------------------------------------------------------------
function CGAPoint:Setup()
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or galua( "0" )
end

------------------------------------------------------------------------
-- Something needs to be said here about dual point and direct points.
-- The null-vectors of CGA are both dual and direct points.  The duals
-- of all null-vectors are also both dual and direct points.  Here we
-- are arbitrarily choosing to call the null-vectors dual points, and
-- their duals the direct points.
function CGAPoint:ComposeBlade()

	-- Formulate a dual point.
	local blade = self.weight * ( no + self.center + 0.5 * ( self.center .. self.center ) * ni )
	
	-- Make it a direct point, if needed.
	if not self.dual then
		blade = blade * I
	end
	
	-- Return a vector representative of the point.
	return blade
	
end

------------------------------------------------------------------------
function CGAPoint:DecomposeBlade( blade )

	-- Try to find the dual point that is the point represented by the given blade.
	local dual = true
	local dualPoint
	local dirPoint = blade[4]
	if dirPoint ~= 0 then
		dualPoint = dirPoint * -I
		dual = false
	else
		dualPoint = blade[1]
	end
	
	-- If we didn't find it, then we were not given a point.
	if dualPoint == 0 then
		return false
	end
	
	-- Decompose the point.
	local weight = dualPoint .. -ni
	if ( #weight ):tonumber() < self.epsilon then
		return false	-- In this case what we have is really a dual plane.
	end
	self.dual = dual
	self.weight = weight
	dualPoint = dualPoint / weight
	self.center = no_ni .. ( dualPoint ^ no_ni )
	
	-- Return success.
	return true
	
end

-- TODO: Implement the rest of the CGA geometries.

------------------------------------------------------------------------
-- The default sphere is a degenerate sphere, which is a point.
function CGASphere:Setup()
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or galua( "0" )
	self.radius = self.radius or 0
	self.imaginary = self.imaginary or false
end

------------------------------------------------------------------------
function CGASphere:ComposeBlade()

	-- Formulate a dual sphere.
	local sign = self.imaginary and -1 or 1
	local blade = self.weight * ( no + self.center + 0.5 * ( self.center .. self.center - sign * self.radius * self.radius ) * ni )
	
	-- Make it a direct sphere, if needed.
	if not self.dual then
		blade = blade * I
	end
	
	-- Return a vector representative of the sphere.
	return blade
	
end

------------------------------------------------------------------------
function CGASphere:DecomposeBlade( blade )

	-- Try to find the dual sphere that is the sphere represented by the given blade.
	local dual = true
	local dualSphere
	local dirSphere = blade[4]
	if dirSphere ~= 0 then
		dualSphere = dirSphere * -I
		dual = false
	else
		dualSphere = blade[1]
	end
	
	-- If we didn't find it, then we were not given a sphere.
	if dualSphere == 0 then
		return false
	end
	
	-- Decompose the sphere.
	local weight = dualSphere .. -ni
	if ( #weight ):tonumber() < self.epsilon then
		return false	-- In this case what we have is really a dual plane.
	end
	self.dual = dual
	self.weight = weight
	dualSphere = dualSphere / weight
	self.center = no_ni .. ( dualSphere ^ no_ni )
	local radicand = ( 2 * dualSphere .. no + self.center * self.center )[0]:tonumber()		-- Take zero part to kill round-off error.
	self.imaginary = false
	if radicand < 0 then
		self.imaginary = true
		radicand = -radicand
	end
	self.radius = math.sqrt( radicand )
	
	-- Return success.
	return true
	
end

------------------------------------------------------------------------
-- Provide a way to create instances of the CGA geometry classes.
function CGAUtil.NewPoint( point )				return CGAPoint:New( point ) end
function CGAUtil.NewFlatPoint( flatPoint )		return CGAFlatPoint:New( flatPoint ) end
function CGAUtil.NewPointPair( pointPair )		return CGAPointPair:New( pointPair ) end
function CGAUtil.NewLine( line )				return CGALine:New( line ) end
function CGAUtil.NewCircle( circle )			return CGACircle:New( circle ) end
function CGAUtil.NewPlane( plane )				return CGAPlane:New( plane ) end
function CGAUtil.NewSphere( sphere )			return CGASphere:New( sphere ) end

------------------------------------------------------------------------
-- Return the API table for this module.
return CGAUtil

-- CGAUtil.lua