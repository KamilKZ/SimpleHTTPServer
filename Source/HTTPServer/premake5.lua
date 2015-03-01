function linkLua( root )
	root = root or DIR_ROOT
	includedirs( root .. LIB_LUA.INCLUDEDIR )
	libdirs( root .. LIB_LUA.LIBDIR )
	
	configuration "vs*"
		links( "luajit" ) --work around for build order/dependancy
		links( LIB_LUA.LIBLINK )
		
	configuration "gmake"
		linkoptions( "-L" .. DIR_ROOT .. LIB_LUA.LIBDIR .. "/" .. LIB_LUA.LIBLINKUNIX )
		--links( LIB_LUA.LIBLINKUNIX )
	
	configuration {}
end

project "HTTPServer"
	language "C++"
	files { "Source/**.*", "Include/**.*" }
	includedirs { "Include" }
	
	kind ("SharedLib")
	
	configMacro()
	
	--LUA - for later
		linkLua(DIR_ROOT)

	--sfml - for networking
		defines { "SFML_STATIC=1" }
		links { "sfml-system", "sfml-network" }
		includedirs { DIR_SOURCE .."SFML/include/" }
	
		configuration { "windows", "vs*" }
			--static lib dependencies
			--libdirs { DIR_SOURCE .. "SFML/extlibs/libs-msvc/x86" }
			links { "ws2_32" }
		configuration {}
	
	targetdir( DIR_LIB )
	location( DIR_BUILD )