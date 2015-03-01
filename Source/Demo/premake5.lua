project "LuaServer"
	language "C++"
	files { "**.cpp" }
	
	kind ("ConsoleApp")
	
	configMacro()
	
	--HTTPServer
		includedirs( DIR_SOURCE .. "HTTPServer/Include" )
		links( "HTTPServer" )
	
	--LUA - for later
		includedirs( DIR_ROOT .. LIB_LUA.INCLUDEDIR )
		libdirs( DIR_ROOT .. LIB_LUA.LIBDIR )
		links( "luajit" ) --work around for build order/dependancy
		links( LIB_LUA.LIBLINK )
	
	--sfml - for networking, is this even still needed?
		defines { "SFML_STATIC=1" }
		includedirs { DIR_SOURCE .."SFML/include/" }
	
	targetdir( DIR_BIN )
	location( DIR_BUILD )