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

local epsilon = 0.000001

------------------------------------------------------------------------
function CompareVectors( gaVector, testVector )

	if ( ( gaVector .. e1 ) - galua( testVector.x ) ):tonumber() >= epsilon then
		return false, "Wrong x-component"
	end
	if ( ( gaVector .. e2 ) - galua( testVector.y ) ):tonumber() >= epsilon then
		return false, "Wrong y-component"
	end
	if ( ( gaVector .. e3 ) - galua( testVector.z ) ):tonumber() >= epsilon then
		return false, "Wrong z-component"
	end
	return true

end

------------------------------------------------------------------------
function VerifyGeoData( cgaGeo, cgaGeoTestData, weightSignMatters )

	if cgaGeoTestData.weight then
		if not cgaGeo.weight then
			return false, "Weight missing"
		end
		if ( galua( cgaGeo.weight ) - galua( cgaGeoTestData.weight ) ):tonumber() >= epsilon then
			if weightSignMatters then
				return false, "Wrong weight"
			elseif ( galua( cgaGeo.weight ) - galua( -cgaGeoTestData.weight ) ):tonumber() >= epsilon then
				return false, "Wrong weight (even after sign change)"
			end
		end
	end
	
	if cgaGeoTestData.center then
		if not cgaGeo.center then
			return false, "Center missing"
		end
		local passFail, failReason = CompareVectors( cgaGeo.center, cgaGeoTestData.center )
		if not passFail then
			return false, "Center: " .. failReason
		end
	end
	
	if cgaGeoTestData.normal then
		if not cgaGeo.normal then
			return false, "Normal missing"
		end
		local passFail, failReason = CompareVectors( cgaGeo.normal, cgaGeoTestData.normal )
		if not passFail then
			return false, "Normal: " .. failReason
		end
	end
	
	if cgaGeoTestData.radius then
		if not cgaGeo.radius then
			return false, "Radius missing"
		end
		if ( galua( cgaGeo.radius ) - galua( cgaGeoTestData.radius ) ):tonumber() >= epsilon then
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
		center = { x = -1, y = 3, z = -2 },
	}
	
	local pointGeo = cga.NewPoint
	{
		weight = pointTestData.weight,
		center = cga.evec( pointTestData.center.x, pointTestData.center.y, pointTestData.center.z )
	}
	
	local passFail, failReason = VerifyGeoData( pointGeo, pointTestData, true )
	if not passFail then
		return false, failReason .. " (Failed basic object construction.)"
	end
	
	local pointBlade = pointGeo:ComposeBlade()
	if not pointGeo:DecomposeBlade( pointBlade ) then
		return false, "Decomposition failed"
	end
	
	passFail, failReason = VerifyGeoData( pointGeo, pointTestData, true )
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
		center = { x = -1, y = 3, z = -2 },
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

	local passFail, failReason = VerifyGeoData( sphereGeo, sphereTestData, true )
	if not passFail then
		return false, failReason .. " (Failed basic object construction.)"
	end
	
	local sphereBlade = sphereGeo:ComposeBlade()
	if not sphereGeo:DecomposeBlade( sphereBlade ) then
		return false, "Decomposition failed"
	end
	
	passFail, failReason = VerifyGeoData( sphereGeo, sphereTestData, true )
	if not passFail then
		return false, failReason .. " (Failed basic decomposition.)"
	end
	
	return true
	
end

------------------------------------------------------------------------
cgaTest = {}
cgaTestList[ #cgaTestList + 1 ] = cgaTest
cgaTest.name = "CGAPlane Trivial Test"
function cgaTest.perform()
	
	local planeTestData =
	{
		weight = -3,
		center = { x = -1, y = 3, z = -2 },
		normal = { x = math.sqrt(2) / 2, y = math.sqrt(2) / 2, z = 0 },
	}
	
	local planeGeo = cga.NewPlane
	{
		weight = planeTestData.weight,
		center = cga.evec( planeTestData.center.x, planeTestData.center.y, planeTestData.center.z ),
		normal = cga.evec( planeTestData.normal.x, planeTestData.normal.y, planeTestData.normal.z ),
	}
	
	local passFail, failReason = VerifyGeoData( planeGeo, planeTestData, true )
	if not passFail then
		return false, failReason .. " (Failed basic object construction.)"
	end
	
	local planeBlade = planeGeo:ComposeBlade()
	if not planeGeo:DecomposeBlade( planeBlade ) then
		return false, "Decomposition failed"
	end
	
	planeTestData.center = nil		-- We'll verify the position seperately.
	passFail, failReason = VerifyGeoData( planeGeo, planeTestData, false )
	if not passFail then
		return false, failReason .. " (Failed basic decomposition.)"
	end
	
	if ( planeGeo.center .. planeBlade ):tonumber() >= epsilon then
		return false, "Decomposed center not on originaly composed plane."
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