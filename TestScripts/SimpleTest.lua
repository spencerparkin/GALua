-- SimpleTest.lua

package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
package.cpath = package.cpath .. ";D:\\GALua\\Build\\Debug\\?.dll"

-- Acquire the GALua API.
local galua = require 'galua'

-- Configure our algebra.
local algebraFile = "../Algebras/EGA3D.lua"
local algebra = dofile( algebraFile )
if not algebra then
	error( "Failed to get algebra function from file \"" .. algebraFile .. "\"." )
end
algebra( galua )

-- Print out an array of multi-vectors.
function PrintMultiVecArray( mvArray )
	for idx, multiVec in ipairs( mvArray ) do
		local result = tostring( multiVec )
		print( idx .. " -> " .. result )
	end
end

-- Test with these multi-vectors.  We will do a series of tests
-- where in each test we generate an array of multi-vectors.
-- Each test should generate the same array of multi-vectors.
local mvStringA = "1 + e2^e2"
local mvStringB = "e1 - 3*e1^e2^e3"

--===========================================================
-- Test API functions.
--===========================================================
print( "Testing API functions..." )
local mvArray = {}
mvArray[ #mvArray + 1 ] = galua.from_string( mvStringA )
mvArray[ #mvArray + 1 ] = galua.from_string( mvStringB )
mvArray[ #mvArray + 1 ] = galua.add( mvArray[1], mvArray[2] )
mvArray[ #mvArray + 1 ] = galua.sub( mvArray[1], mvArray[2] )
mvArray[ #mvArray + 1 ] = galua.gp( mvArray[1], mvArray[2] )
mvArray[ #mvArray + 1 ] = galua.ip( mvArray[1], mvArray[2] )
mvArray[ #mvArray + 1 ] = galua.op( mvArray[1], mvArray[2] )
mvArray[ #mvArray + 1 ] = galua.negate( mvArray[1] )
mvArray[ #mvArray + 1 ] = galua.reverse( mvArray[2] )
mvArray[ #mvArray + 1 ] = galua.get_grade_part( mvArray[2], 3 )
local mag = galua.mag( mvArray[1] )
PrintMultiVecArray( mvArray )
print( "mag = " .. galua.to_string( mag ) )

--===========================================================
-- Test operator overload functions.
--===========================================================
print( "Testing operator overload functions..." )
mvArray = {}
mvArray[ #mvArray + 1 ] = galua( mvStringA )
mvArray[ #mvArray + 1 ] = galua( mvStringB )
mvArray[ #mvArray + 1 ] = mvArray[1] + mvArray[2]
mvArray[ #mvArray + 1 ] = mvArray[1] - mvArray[2]
mvArray[ #mvArray + 1 ] = mvArray[1] * mvArray[2]
mvArray[ #mvArray + 1 ] = mvArray[1] % mvArray[2]
mvArray[ #mvArray + 1 ] = mvArray[1] ^ mvArray[2]
mvArray[ #mvArray + 1 ] = -mvArray[1]
mvArray[ #mvArray + 1 ] = galua.reverse( mvArray[2] )	-- No overload for this.
mvArray[ #mvArray + 1 ] = mvArray[2][3]
local mag = #mvArray[1]
PrintMultiVecArray( mvArray )
print( "mag = " .. tostring( mag ) )

--===========================================================
-- Test coercion.
--===========================================================
print( "Testing coercion..." )
mvArray = {}
mvArray[ #mvArray + 1 ] = galua( mvStringA )
mvArray[ #mvArray + 1 ] = galua( mvStringB )
mvArray[ #mvArray + 1 ] = mvStringA + mvArray[2]
mvArray[ #mvArray + 1 ] = mvArray[1] - mvStringB
mvArray[ #mvArray + 1 ] = galua.gp( mvStringA, mvStringB )
mvArray[ #mvArray + 1 ] = mvStringA % mvArray[2]
mvArray[ #mvArray + 1 ] = mvArray[1] ^ mvStringB
mvArray[ #mvArray + 1 ] = galua.negate( mvStringA )
mvArray[ #mvArray + 1 ] = galua.reverse( mvStringB )
mvArray[ #mvArray + 1 ] = galua.get_grade_part( mvStringB, 3 )
local mag = galua.mag( mvStringA )
PrintMultiVecArray( mvArray )
print( "mag = " .. galua.to_string( mag ) )

-- SimpleTest.lua