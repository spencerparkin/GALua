-- CGAUtil.lua

--[[

TODO: Make this a lua-based module that, given a GALua API, can help the user
      compose and decompose CGA geometries such as spheres, lines, planes, circles, etc.

sphere = cgautil.compose_sphere( pos, radius )
pos, radius = cgautil.decompose_sphere( sphere )
	  
--]]

-- CGAUtil.lua