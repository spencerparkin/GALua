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
		if not cgaGeo.weight then
			return false, "Weight missing"
		end
		if galua( cgaGeo.weight ) ~= galua( cgaGeoTestData.weight ) then
			return false, "Wrong weight"
		end
	end
	
	if cgaGeoTestData.center then
		if not cgaGeo.center then
			return false, "Center missing"
		end
		if ( cgaGeo.center .. e1 ) ~= galua( cgaGeoTestData.center.x ) then
			return false, "Center: Wrong x-component"
		end
		if ( cgaGeo.center .. e2 ) ~= galua( cgaGeoTestData.center.y ) then
			return false, "Center: Wrong y-component"
		end
		if ( cgaGeo.center .. e3 ) ~= galua( cgaGeoTestData.center.z ) then
			return false, "Center: Wrong z-component"
		end
	end
	
	if cgaGeoTestData.normal then
		if not cgaGeo.normal then
			return false, "Normal missing"
		end
		if ( cgaGeo.normal .. e1 ) ~= galua( cgaGeoTestData.normal.x ) then
			return false, "Normal: Wrong x-component"
		end
		if ( cgaGeo.normal .. e2 ) ~= galua( cgaGeoTestData.normal.y ) then
			return false, "Normal: Wrong y-component"
		end
		if ( cgaGeo.normal .. e3 ) ~= galua( cgaGeoTestData.normal.z ) then
			return false, "Normal: Wrong z-component"
		end
	end
	
	if cgaGeoTestData.radius then
		if not cgaGeo.radius then
			return false, "Radius missing"
		end
		if galua( cgaGeo.radius ) ~= galua( cgaGeoTestData.radius ) then
			return false, "Wrong radius: " .. tostring( cgaGeo.radius ) .. " ~= " .. tostring( cgaGeoTestData.radius )
		end
	end
	
	if cgaGeoTestData.imaginary ~= nil then
		if not cgaGeo.imaginary then
			return false, "Imaginary missing"
		end
		if cgaGeo.imaginary ~= cgaGeoTestData.imaginary then
			return false, "Wrong imaginary"
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
		imaginary = true,
	}
	
	local sphereGeo = cga.NewSphere
	{
		weight = sphereTestData.weight,
		center = cga.evec( sphereTestData.center.x, sphereTestData.center.y, sphereTestData.center.z ),
		radius = sphereTestData.radius,
		imaginary = sphereTestData.imaginary,
	}

	local passFail, failReason = VerifyGeoData( sphereGeo, sphereTestData )
	if not passFail then
		return false, failReason .. " (Failed basic object construction.)"
	end
	
	local sphereBlade = sphereGeo:ComposeBlade()
	if not sphereGeo:DecomposeBlade( sphereBlade ) then
		return false, "Decomposition failed"
	end
	
	local passFail, failReason = VerifyGeoData( sphereGeo, sphereTestData )
	if not passFail then
		return false, failReason .. " (Failed basic decomposition.)"
	end
	
	return true
	
end

-- TODO: Put more tests here...

------------------------------------------------------------------------
for _, cgaTest in ipairs( cgaTestList ) do
	local testName = cgaTest.name
	local passFail, failReason = cgaTest.perform()
	local result = testName .. ": " .. ( passFail and "Passed." or "FAILED!" )
	if not passFail then
		result = result .. " -- " .. failReason
	end
	print( result )
end

-- TestCGAUtil.lua