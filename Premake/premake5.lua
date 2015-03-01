local BUILD = true

newaction {
	trigger = "clean",
	description = "Clean up temporary files",
	execute = function()
		os.rmdir("..\Build")
		os.execute("cd ../Bin&&del *.obj /Q&&del *.pdb /Q&&del *.exp /Q&&del *.idb /Q&&del *.ilk /Q")
	end
}

newaction {
	trigger = "clean-all",
	description = "Clean up temporary and binary files",
	execute = function()
		os.rmdir("../Build")
		os.rmdir("../Bin")
		os.rmdir("../Lib")
		--os.execute("cd ..\Bin&&del *.* /Q") --remove all files from /Bin/ but not folders
	end
}

newaction {
	trigger = "version",
	description = "Get the version of the sources",
	execute = function()
		print("SimpleHTTPServer: v0.1.1\n\tSFML: v2.1\n\tLuaJIT: v2.0.3")
	end
}

newaction {
	trigger = "gen-docs",
	description = "Generate documentation",
	execute = function()
		os.execute("cd ../Docs&&doxygen Doxyfile.cfg")
	end
}

DIR_ROOT = "../../"
DIR_BIN 			= DIR_ROOT .. "Bin/"
DIR_BUILD 			= DIR_ROOT .. "Build/"
DIR_LIB 			= DIR_ROOT .. "Lib/"
DIR_SOURCE 			= DIR_ROOT .. "Source/"

BDIR_ROOT = "../"
BDIR_BIN 		= BDIR_ROOT .. "Bin/"
BDIR_LIB 		= BDIR_ROOT .. "Lib/"
BDIR_BUILD 		= BDIR_ROOT .. "Build/"
BDIR_SOURCE 	= BDIR_ROOT .. "Source/"
	
LIB_LUA	= {}
	LIB_LUA.INCLUDEDIR 	= "Source/luajit/src"
	LIB_LUA.LIBDIR 		= "Source/luajit/src"
	LIB_LUA.LIBLINK 	= { "lua51.lib" }
	LIB_LUA.LIBLINKUNIX = "libluajit.a"
	
PREMAKE_DEBUG = false

package.path = package.path .. ";?.lua"

if _ACTION == "clean" or
	_ACTION == "clean-all" or
	_ACTION == "version" or
	_ACTION == "gen-docs" then
	--do nothing
else

function configMacro()
	configuration "Release**"
		defines { "NDEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		optimize "On"
		
	configuration "Debug**"
		flags { "Symbols" }
		defines { "DEBUG=1", "_DEBUG=1", "_ITERATOR_DEBUG_LEVEL=2" }
		optimize "Off"
		
	configuration "RelWithDebInfo**"
		flags { "Symbols" }
		defines { "NDEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		optimize "On" -- or "Debug"?
		
	configuration { "gmake", "StaticLib" }
		buildoptions { "-fPIC" }
	
	configuration { "gmake" }
		buildoptions { "-std=c++11", "-Wno-unknown-pragmas", "-Wno-unused-parameter" }
		
	configuration {}
end

solution "ALL_BUILD"
	configurations {"Release", "Debug", "RelWithDebInfo" }
	platforms {"x32", "x64"}
	
	configMacro()
		
	configuration "vs*"
		--buildoptions { "/INCREMENTAL:NO" } --why not?
		defines { "_CRT_SECURE_NO_DEPRECATE" }
		defines { "_CRT_SECURE_NO_WARNINGS" }
		
	configuration "windows"
		defines { "WIN32", "_WIN32", "__OS=\"Windows\"" }

	configuration "macosx"
		defines { "MACOSX", "__OS=\"MacOSX\"" }

	configuration "linux"
		defines { "LINUX", "__OS=\"Linux\"" }
		
	configuration "x32"
		vectorextensions "SSE2" --Enable SSE2 cpu extensions
		--vectorextensions "AVX" --Enable AXV cpu extensions; 2011+ Bulldozer/Sandy Bridge
	
	configuration {}
		
	targetdir( BDIR_LIB ) --put it in lib instead..
	location( BDIR_BUILD )
	
	include ( BDIR_SOURCE .. "Demo/" )
	include ( BDIR_SOURCE .. "HTTPServer/" )
	include ( BDIR_SOURCE .. "luajit/" )
	include ( BDIR_SOURCE .. "SFML/" )
	
end --if action == clean, clean-all, version; else.