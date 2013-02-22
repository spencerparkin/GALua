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
	self.epsilon = 0.0001
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
-- Notice that we do not attempt to check whether the given multi-vector
-- value is homogeneous of a given grade.  We assume that it is.
-- Furthermore, we assume that it is such an element that may be factored
-- as a blade.  If these assumptions are not met, the result of this
-- function is simply undefined.
function CGAGeometry:GrabDualGeometry( blade, dualGrade )
	
	-- Ultimately we want to return a dual geometry.
	local dualGeo
	
	-- We're assuming that the given blade dually represents the geometry.
	local dual = true
	
	-- Calculate the grade of the blade directly repreentative of the geometry.
	local dirGrade = 5 - dualGrade
	
	-- Is the blade of the direct grade?
	local dirGeo = blade[ dirGrade ]
	if dirGeo ~= 0 then
	
		-- Yes.  We were given a blade directly representative of the geometry.
		dualGeo = dirGeo * -I
		dual = false
		
	else
	
		-- No.  It then must be of the dual grade.
		dualGeo = blade[1]
		
	end
	
	-- Return the geometry in dual form and whether or not we were given the blade in dual form.
	return dualGeo, dual
	
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
	self.type = "point"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
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
	-- If we didn't find it, then we were not given a point.
	local dualPoint, dual = self:GrabDualGeometry( blade, 1 )
	if dualPoint == 0 then
		return false
	end
	
	-- Decompose the point.
	local weight = dualPoint .. -ni
	if ( #weight ):tonumber() < self.epsilon then
		return false	-- In this case what we have is really a dual plane.
	end
	dualPoint = dualPoint / weight
	local center = no_ni .. ( dualPoint ^ no_ni )
	if ( #( -2 * dualPoint .. no - center * center ) ):tonumber() >= self.epsilon then
		return false	-- In thsi case what we have is really a dual sphere.
	end
	
	-- Store the decomposition.
	self.dual = dual
	self.weight = weight
	self.center = center
	
	-- Return success.
	return true
	
end

------------------------------------------------------------------------
function CGAFlatPoint:Setup()
	self.type = "flatpoint"
end

------------------------------------------------------------------------
function CGAFlatPoint:ComposeBlade()
end

------------------------------------------------------------------------
function CGAFlatPoint:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGAPointPair:Setup()
	self.type = "pointpair"
end

------------------------------------------------------------------------
function CGAPointPair:ComposeBlade()
end

------------------------------------------------------------------------
function CGAPointPair:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGALine:Setup()
	self.type = "line"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
end

------------------------------------------------------------------------
function CGALine:ComposeBlade()

	-- Formulate a dual or direct line.
	if self.dual then
		blade = self.weight * ( self.normal + self.center ^ self.normal ^ ni ) * Ie
	else
		blade = self.weight * ( self.normal + self.center ^ self.normal ^ ni ) * -no_ni
	end
	
	-- Return the blade representative of the line.
	return blade
	
end

------------------------------------------------------------------------
function CGALine:DecomposeBlade( blade )

	-- Try to find the dual line that is the line represented by the given blade.
	-- If we didn't find it, then we were not give a line.
	local dual = true
	local dualLine, dual = self:GrabDualGeometry( blade, 2 )
	
	-- Decompose the line.
	local normal = ( no .. ( ( dualLine * -Ie ) ^ ni ) )[1]		-- Take vector part to kill round-off error.
	local weight = #normal
	if weight:tonumber() < self.epsilon then
		return false		-- In this case, I believe that we may have been given is a free blade.
	end
	normal = normal / weight
	dualLine = dualLine / weight
	local center = ( -( no .. L ) * normal )[1]		-- This will be the point on the line closest to the origin.
	
	-- TODO: How else do we need to return false if we're not actually a dual line?
	
	-- Store the decomposition.
	self.dual = dual
	self.weight = weight
	self.normal = normal
	self.center = center
	
	-- Return success.
	return true
	
end

------------------------------------------------------------------------
function CGACircle:Setup()
	self.type = "circle"
end

------------------------------------------------------------------------
function CGACircle:ComposeBlade()
end

------------------------------------------------------------------------
function CGACircle:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGAPlane:Setup()
	self.type = "plane"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
end

------------------------------------------------------------------------
function CGAPlane:ComposeBlade()

	-- Formulate a dual plane.
	local blade = self.weight * ( self.normal + ( self.normal .. self.center ) * ni )
	
	-- Make it a direct plane, if needed.
	if not self.dual then
		blade = blade * I
	end
	
	-- Return a vector representative of the plane.
	return blade
	
end

------------------------------------------------------------------------
function CGAPlane:DecomposeBlade( blade )

	-- Try to find the dual plane that is the plane represented by the given blade.
	-- If we didn't find it, then we were not give a plane.
	local dualPlane, dual = self:GrabDualGeometry( blade, 1 )
	if dualPlane == 0 then
		return false
	end
	
	-- Decompose the plane.
	if ( dualPlane .. ni ) ~= 0 then
		return false		-- In this case, we were given a point or sphere.
	end
	local normal = no .. ( dualPlane ^ ni )
	local weight = #normal
	if weight:tonumber() < self.epsilon then
		return false		-- In this case, we were given the null-point at infinity.
	end
	normal = normal / weight
	dualPlane = dualPlane / weight
	local center = ( -( dualPlane .. no ) * normal )[1]		-- This will be the point on the plane closest to the origin.
	
	-- Store the decomposition.
	self.dual = dual
	self.weight = weight
	self.normal = normal
	self.center = center
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
-- The default sphere is a degenerate sphere, which is a point.
function CGASphere:Setup()
	self.type = "sphere"
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
-- Note that if we were really given a point, we simply return a
-- degenerate sphere.  So any check for geometric type should test
-- points before spheres.
function CGASphere:DecomposeBlade( blade )

	-- Try to find the dual sphere that is the sphere represented by the given blade.
	-- If we didn't find it, then we were not given a sphere.
	local dualSphere, dual = self:GrabDualGeometry( blade, 1 )
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