-- CGAUtil.lua

------------------------------------------------------------------------
local CGAUtil = {}

------------------------------------------------------------------------
local galua = nil

------------------------------------------------------------------------
local e1, e2, e3, no, ni
local Ie, no_ni, I

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
	Ie = e1 ^ e2 ^ e3
	I = Ie ^ no_ni
	
end

------------------------------------------------------------------------
function CGAUtil.evec( x, y, z )
	return x*e1 + y*e2 + z*e3
end

------------------------------------------------------------------------
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
local CGAPoint = CGAGeometry:New()
local CGAFlatPoint = CGAGeometry:New()
local CGAPointPair = CGAGeometry:New()
local CGALine = CGAGeometry:New()
local CGACircle = CGAGeometry:New()
local CGAPlane = CGAGeometry:New()
local CGASphere = CGAGeometry:New()

--------------------
-- GEOMETRY SETUP --
--------------------

------------------------------------------------------------------------
function CGAPoint:Setup()
	self.type = "point"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
end

------------------------------------------------------------------------
function CGAFlatPoint:Setup()
	self.type = "flatpoint"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
end

------------------------------------------------------------------------
function CGAPointPair:Setup()
	self.type = "pointpair"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
	self.radius = self.radius or 1
	self.imaginary = self.imaginary or false
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
function CGACircle:Setup()
	self.type = "circle"
	self.dual = self.dual or true
	self.weight = self.weight or 1
	self.center = self.center or 0
	self.normal = self.normal or e2
	self.radius = self.radius or 1
	self.imaginary = self.imaginary or false
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
function CGASphere:Setup()
	self.type = "sphere"
	self.dual = self.dual or true
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
end

------------------------------------------------------------------------
function CGAFlatPoint:ComposeBlade()
end

------------------------------------------------------------------------
function CGAPointPair:ComposeBlade()
end

------------------------------------------------------------------------
function CGALine:ComposeBlade()
end

------------------------------------------------------------------------
function CGACircle:ComposeBlade()
end

------------------------------------------------------------------------
function CGAPlane:ComposeBlade()
end

------------------------------------------------------------------------
function CGASphere:ComposeBlade()
end

----------------------------
-- GEOMETRY DECOMPOSITION --
----------------------------

------------------------------------------------------------------------
function CGAPoint:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGAFlatPoint:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGAPointPair:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGALine:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGACircle:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGAPlane:DecomposeBlade( blade )
end

------------------------------------------------------------------------
function CGASphere:DecomposeBlade( blade )
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
return CGAUtil

-- CGAUtil.lua