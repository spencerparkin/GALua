-- Test the GALua module.

--package.cpath = package.cpath .. ";.\\Debug\\?.dll"
package.cpath = package.cpath .. ";C:\\GALua\\Build\\Debug\\?.dll"
print( package.cpath )

-- Acquire the GALua API.
local galua = require 'galua'

-- Define our basis vectors.  These span the vector space
-- that generates the geometric algebra that we will use.
local basisVecs = { "e1", "e2", "e3" }
galua.def_basis( basisVecs )

-- Define the signature of our GA.
local basisVecIPFunc = function( i, j )
	local ip_table =
	{
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	}
	return ip_table[i][j]
end
galua.def_sig( basisVecIPFunc )

-- We're now ready to do some GA!
local multiVecA = galua.from_string( "3*e1 + 2*e1^e2 - 4" )
local multiVecB = galua.from_string( "-2 + e3 + e3^e1" )
local multiVecC = galua.sum( multiVecA, multiVecB )
local result = galua.to_string( multiVecC )
print( "result = " .. result )