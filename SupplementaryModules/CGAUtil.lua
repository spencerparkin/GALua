-- CGAUtil.lua

------------------------------------------------------------------------
local CGAUtil = {}

------------------------------------------------------------------------
local galua = nil

------------------------------------------------------------------------
local e1, e2, e3, no, ni
local i, no_ni, I

------------------------------------------------------------------------
-- This needs to be called before the module gets used!
function CGAUtil.Setup( galua_api )
	
	galua = galua_api
	
	e1 = galua( "e1" )
	e2 = galua( "e2" )
	e3 = galua( "e3" )
	no = galua( "no" )
	ni = galua( "ni" )
	no_ni = no ^ ni
	i = e1 ^ e2 ^ e3
	I = i ^ no_ni
	
end

------------------------------------------------------------------------
function CGAUtil.evec( x, y, z )
	return x*e1 + y*e2 + z*e3
end

------------------------------------------------------------------------
local CGAGeometry = {}

------------------------------------------------------------------------
function CGAGeometry:Default()
	self.epsilon = 0.0001
end

------------------------------------------------------------------------
function CGAGeometry:New( arg )
	local geo
	if type( arg ) == "table" then
		geo = arg
	else
		geo = {}
	end
	setmetatable( geo, self )
	self.__index = self
	geo:Default()
	if type( arg ) == "userdata" then
		if not geo:DecomposeBlade( arg ) then
			geo = nil
		end
	end
	return geo
end

------------------------------------------------------------------------
local CGAPoint = CGAGeometry:New()
local CGAFlatPoint = CGAGeometry:New()
local CGAPointPair = CGAGeometry:New()
local CGALine = CGAGeometry:New()
local CGACircle = CGAGeometry:New()
local CGAPlane = CGAGeometry:New()
local CGASphere = CGAGeometry:New()

-----------------------
-- GEOMETRY DEFAULTS --
-----------------------

------------------------------------------------------------------------
function CGAPoint:Default()
	self.type = "point"
	self.weight = self.weight or 1
	self.center = self.center or 0
end

------------------------------------------------------------------------
function CGAFlatPoint:Default()
	self.type = "flatpoint"
	self.weight = self.weight or 1
	self.center = self.center or 0
end

------------------------------------------------------------------------
function CGAPointPair:Default()
	self.type = "pointpair"
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
	self.radius = self.radius or 1
	self.imaginary = self.imaginary or false
end

------------------------------------------------------------------------
function CGALine:Default()
	self.type = "line"
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
end

------------------------------------------------------------------------
function CGACircle:Default()
	self.type = "circle"
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
	self.radius = self.radius or 1
	self.imaginary = self.imaginary or false
end

------------------------------------------------------------------------
function CGAPlane:Default()
	self.type = "plane"
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
end

------------------------------------------------------------------------
function CGASphere:Default()
	self.type = "sphere"
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.radius = self.radius or 1
	self.imaginary = self.imaginary or false
end

--------------------------
-- GEOMETRY COMPOSITION --
--------------------------

------------------------------------------------------------------------
function CGAPoint:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local blade = weight * ( no + center + 0.5 * ( center .. center ) * ni )
	return blade
end

------------------------------------------------------------------------
function CGAFlatPoint:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local blade = weight * ( 1 - center ^ ni ) * i
	return blade
end

------------------------------------------------------------------------
function CGAPointPair:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local normal = self.normal
	local radius = self.radius
	local sign = self.imaginary and -1 or 1
	local blade = weight * ( no ^ normal + center ^ normal ^ no_ni - ( center .. normal ) -
			( ( center .. normal ) * center - 0.5 * ( ( center .. center ) + sign * radius * radius ) * normal ) ^ ni ) * i
	return blade
end

------------------------------------------------------------------------
function CGALine:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local normal = self.normal
	local blade = weight * ( normal + ( center ^ normal ) * ni ) * i
	return blade
end

------------------------------------------------------------------------
function CGACircle:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local normal = self.normal
	local radius = self.radius
	local sign = self.imaginary and -1 or 1
	local blade = weight * ( no ^ normal + ( center .. normal ) * no_ni + center ^ normal +
			( ( center .. normal ) * center - 0.5 * ( ( center .. center ) - sign * radius * radius ) * normal ) ^ ni )
	return blade
end

------------------------------------------------------------------------
function CGAPlane:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local normal = self.normal
	local blade = weight * ( normal + ( center .. normal ) * ni )
	return blade
end

------------------------------------------------------------------------
function CGASphere:ComposeBlade()
	local weight = self.weight
	local center = self.center
	local radius = self.radius
	local sign = self.imaginary and -1 or 1
	local blade = weight * ( no + center + 0.5 * ( ( center .. center ) - sign * radius * radius ) * ni )
	return blade
end

----------------------------
-- GEOMETRY DECOMPOSITION --
----------------------------

------------------------------------------------------------------------
function CGAPoint:DecomposeBlade( blade )

	-- It must be non-zero and of grade 1.
	blade = blade[1]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = -blade .. ni
	if weight == 0 then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local center = no_ni .. ( blade ^ no_ni )
	
	-- It must be a degenerate sphere.
	if ( #( ( center .. center ) + 2 * ( no .. blade ) ) ):tonumber() >= self.epsilon then
		return false
	end
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
function CGAFlatPoint:DecomposeBlade( blade )

	-- It must be non-zero and of grade 3.  We can only hope it's a 3-blade.
	blade = blade[3]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = -( no .. ( blade ^ ni ) ) * i
	if weight == 0 then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local center = ( no .. blade ) * i
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
function CGAPointPair:DecomposeBlade( blade )

	-- It must be non-zero and of grade 3.  We can only hope it's a 3-blade.
	blade = blade[3]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = ( #( ( no_ni .. ( blade ^ ni ) ) * i ) ):tonumber()
	if weight == 0 then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local normal = -( no_ni .. ( blade ^ ni ) ) * i
	local center = -normal * ( no_ni .. ( blade ^ ( no * ni ) ) ) * i
	local radius_squared = -center .. center + 2 * ( ( no_ni .. ( no ^ blade ) ) * i + ( center .. normal ) * center ) * normal
	radius_squared = radius_squared[0]		-- Kill any round-off error
	radius_squared = radius_squared:tonumber()
	local imaginary = false
	if radius_squared < 0 then
		imaginary = true
		radius_squared = -radius_squared
	end
	local radius = math.sqrt( radius_squared )
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	self.normal = normal
	self.radius = radius
	self.imaginary = imaginary
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
function CGALine:DecomposeBlade( blade )

	-- It must be non-zero and of grade 2.  We can only hope it's a 2-blade.
	blade = blade[2]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = ( #( ( no .. ( blade ^ ni ) ) * i ) ):tonumber()
	if weight == 0 then
		return false
	end
	
	-- It must have the right form.
	if blade .. ni ~= galua( 0 ) then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local normal = ( no .. ( blade ^ ni ) ) * i
	local center = ( no .. blade ) * normal * i
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	self.normal = normal
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
function CGACircle:DecomposeBlade( blade )

	-- It must be non-zero and of grade 2.  We can only hope it's a 2-blade.
	blade = blade[2]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = ( #( no_ni .. ( blade ^ ni ) ) ):tonumber()
	if weight == 0 then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local normal = -no_ni .. ( blade ^ ni )
	local center = -normal * ( no_ni .. ( blade ^ ( no * ni ) ) )
	local radius_squared = center .. center - 2 * ( ( no_ni .. ( no ^ blade ) ) + ( center .. normal ) * center ) * normal
	radius_squared = radius_squared[0]		-- Kill any round-off error
	radius_squared = radius_squared:tonumber()
	local imaginary = false
	if radius_squared < 0 then
		imaginary = true
	end
	local radius = math.sqrt( radius_squared )
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	self.normal = normal
	self.radius = radius
	self.imaginary = imaginary
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
function CGAPlane:DecomposeBlade( blade )

	-- It must be non-zero and of grade 1.
	blade = blade[1]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = ( #( no .. ( blade ^ ni ) ) ):tonumber()
	if weight == 0 then
		return false
	end
	
	-- It must have the right form.
	if blade .. ni ~= galua( 0 ) then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local normal = no .. ( blade ^ ni )
	local center = -( no .. blade ) * normal
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	self.normal = normal
	
	-- Return success.
	return true

end

------------------------------------------------------------------------
function CGASphere:DecomposeBlade( blade )

	-- It must be non-zero and of grade 1.
	blade = blade[1]
	if blade == 0 then
		return false
	end
	
	-- It must have non-zero weight.
	local weight = -blade .. ni
	if weight == 0 then
		return false
	end
	
	-- Perform the decomposition.
	blade = blade / weight
	local center = no_ni .. ( blade ^ no_ni )
	local radius_squared = ( center .. center ) + 2 * ( no .. blade )
	radius_squared = radius_squared:tonumber()
	local imaginary = false
	if radius_squared < 0 then
		imaginary = true
		radius_squared = -radius_squared
	end
	local radius = math.sqrt( radius_squared )
	
	-- Store the decomposition.
	self.weight = weight
	self.center = center
	self.radius = radius
	self.imaginary = imaginary
	
	-- Return success.
	return true

end

----------------------
-- MODULE INTERFACE --
----------------------

------------------------------------------------------------------------
function CGAUtil.NewPoint( point )				return CGAPoint:New( point ) end
function CGAUtil.NewFlatPoint( flatPoint )		return CGAFlatPoint:New( flatPoint ) end
function CGAUtil.NewPointPair( pointPair )		return CGAPointPair:New( pointPair ) end
function CGAUtil.NewLine( line )				return CGALine:New( line ) end
function CGAUtil.NewCircle( circle )			return CGACircle:New( circle ) end
function CGAUtil.NewPlane( plane )				return CGAPlane:New( plane ) end
function CGAUtil.NewSphere( sphere )			return CGASphere:New( sphere ) end

------------------------------------------------------------------------
function CGAUtil.Decompose( blade )
	
	local geometryClassList = {}
	geometryClassList[ #geometryClassList + 1 ] = CGAPoint
	geometryClassList[ #geometryClassList + 1 ] = CGAFlatPoint
	geometryClassList[ #geometryClassList + 1 ] = CGAPointPair
	geometryClassList[ #geometryClassList + 1 ] = CGALine
	geometryClassList[ #geometryClassList + 1 ] = CGACircle
	geometryClassList[ #geometryClassList + 1 ] = CGASphere
	
	local geometryList = {}
	for _, geometryClass in ipairs( geometryList ) do
		local geometry = geometryClass:New()
		if geometry:Decompose( blade ) then
			geometryList[ #geometryList + 1 ] = geometry
		end
	end
	return geometryList
	
end

------------------------------------------------------------------------
return CGAUtil

-- CGAUtil.lua