-- GradePart.lua

local galua = require 'galua'
local gautil = require 'gautil'

gautil.EGA3D( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

local E = -5 + e1 - 2*e2 + e1^e2 - 3*e3^e2 + 3*e1^e2^e3
print( "E = " .. tostring(E) )
local E0 = galua.get_grade_part( E, 0 )
local E1 = galua.get_grade_part( E, 1 )
local E2 = galua.get_grade_part( E, 2 )
local E3 = galua.get_grade_part( E, 3 )
print( "E0 = " .. tostring(E0) )
print( "E1 = " .. tostring(E1) )
print( "E2 = " .. tostring(E2) )
print( "E3 = " .. tostring(E3) )
local E0 = E[0]
local E1 = E[1]
local E2 = E[2]
local E3 = E[3]
print( "Now using __index, we get..." )
print( "E0 = " .. tostring(E0) )
print( "E1 = " .. tostring(E1) )
print( "E2 = " .. tostring(E2) )
print( "E3 = " .. tostring(E3) )

print( "Now test modifications of parts..." )
E[0] = 1
print( "E = " .. tostring(E) )
E[1] = 0
print( "E = " .. tostring(E) )
E[2] = e1^e3*-4
print( "E = " .. tostring(E) )
E[3] = e1^e3^e2
print( "E = " .. tostring(E) )

-- GradePart.lua