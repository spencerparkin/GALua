-- Inverse.lua

local galua = require 'galua'
local gautil = require 'gautil'

gautil.EGA3D( galua )

local e1 = galua( "e1" )
local e2 = galua( "e2" )
local e3 = galua( "e3" )

local E = 3 + e1 + e1*e2
local E_inv_l = galua.left_inverse(E)
local E_inv_r = galua.right_inverse(E)
print( "E = " .. tostring(E) )
print( "E_inv_l = " .. tostring( E_inv_l ) )
print( "E_inv_r = " .. tostring( E_inv_r ) )
local E_l_check = E_inv_l * E
local E_r_check = E * E_inv_r
print( "E_l_check = " .. tostring( E_l_check ) )
print( "E_r_check = " .. tostring( E_r_check ) )

local E_scaled = E / 3
print( "E_scaled = " .. tostring( E_scaled ) )

local vec = e1 + e2 + e3
local norm_vec = vec / #vec
print( "norm_vec = " .. tostring( norm_vec ) )
local one = norm_vec * norm_vec
print( "one = " .. tostring( one ) )

-- Inverse.lua