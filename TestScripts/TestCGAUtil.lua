-- TestCGAUtil.lua

local galua = require 'galua'
local gautil = require 'gautil'
local cga = require 'cgautil'

gautil.CGA3D( galua )
cga.Setup( galua )

local cgaTestList = {}
local cgaTest

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

------------------------------------------------------------------------
function VerifyGeoData( cgaGeo, cgaGeoTestData )

	if cgaGeoTestData.weight then
		if cgaGeo.weight:tonumber() ~= cgaGeoTestData.weight then
			return false, "Wrong weight"
		end
	end
	
	if cgaGeoTestData.center then
		if ( cgaGeo.center .. e1 ):tonumber() ~= cgaGeoTestData.center.x then
			return false, "Center: Wrong x-component"
		end
		if ( cgaGeo.center .. e2 ):tonumber() ~= cgaGeoTestData.center.y then
			return false, "Center: Wrong y-component"
		end
		if ( cgaGeo.center .. e3 ):tonumber() ~= cgaGeoTestData.center.z then
			return false, "Center: Wrong z-component"
		end
	end
	
	if cgaGeoTestData.normal then
		if ( cgaGeo.normal .. e1 ):tonumber() ~= cgaGeoTestData.normal.x then
			return false, "Normal: Wrong x-component"
		end
		if ( cgaGeo.normal .. e2 ):tonumber() ~= cgaGeoTestData.normal.y then
			return false, "Normal: Wrong y-component"
		end
		if ( cgaGeo.normal .. e3 ):tonumber() ~= cgaGeoTestData.normal.z then
			return false, "Normal: Wrong z-component"
		end
	end
	
	if cgaGeoTestData.radius then
		if cgaGeo.radius:tonumber() ~= cgaGeoTestData.radius then
			return false
		end
	end
	
	if cgaGeoTestData.imaginary ~= nil then
		if cgaGeo.imaginary ~= cgaGeoTestData.imaginary then
			return false
		end
	end
	
	return true
	
end

------------------------------------------------------------------------
cgaTest = {}
cgaTestList[ #cgaTestList + 1 ] = cgaTest
cgaTest.name = "CGAPoint Trivial Test"
function cgaTest.perform()

	local pointTestData =
	{
		weight = -3,
		center = { x = -1, y = -3, z = -2 },
	}
	
	local pointGeo = cga.NewPoint
	{
		weight = pointTestData.weight,
		center = cga.evec( pointTestData.center.x, pointTestData.center.y, pointTestData.center.z )
	}
	
	local passFail, failReason = VerifyGeoData( pointGeo, pointTestData )
	if not passFail then
		return false, failReason .. " (Failed basic object construction.)"
	end
	
	local pointBlade = pointGeo:ComposeBlade()
	if not pointGeo:DecomposeBlade( pointBlade ) then
		return false, "Decomposition failed"
	end
	
	passFail, failReason = VerifyGeoData( pointGeo, pointTestData )
	if not passFail then
		return false, failReason .. " (Failed basic decomposition.)"
	end
	
	return true
	
end

------------------------------------------------------------------------
cgaTest = {}
cgaTestList[ #cgaTestList + 1 ] = cgaTest
cgaTest.name = "CGASphere Trivial Test"
function cgaTest.perform()

	local sphereTestData =
	{
		weight = -3,
		center = { x = -1, y = -3, z = -2 },
		radius = 8,
	}
	
	local sphereGeo = cga.NewSphere
	{
		weight = sphereTestData.weight,
		center = cga.evec( sphereTestData.center.x, sphereTestData.center.y, sphereTestData.center.z ),
		radius = sphereTestData.radius
	}

	local passFail, failReason = VerifyGeoData( sphereGeo, sphereTestData )
	if not passFail then
		return false, failReason .. " (Failed basic object construction.)"
	end
	
	local sphereBlade = sphereGeo:ComposeBlade()
	if not sphereBlade:DecomposeBlade( sphereBlade ) then
		return false, "Decomposition failed"
	end
	
	local passFail, failReason = VerifyGeoData( sphereGeo, sphereTestData )
	if not passFail then
		return false, failReason .. " (Failed basic decomposition.)"
	end
	
end

-- Put more tests here...

------------------------------------------------------------------------
for _, cgaTest in ipairs( cgaTestList ) do
	local testName = cgaTest.name
	local passFail, failReason = cgaTest.perform()
	print( testName .. ": " .. ( passFail and "Passed." or "FAILED!" ) )
end

-- TestCGAUtil.lua