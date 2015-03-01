project "luajit"
			
	kind ("Makefile")

	configuration {"vs*", "x64"}
		buildcommands { "setenv /release /x64" }
		rebuildcommands { "setenv /release /x64" }
	
	configuration {"vs*"}
	
		buildcommands {
			"cd " ..BDIR_SOURCE.."luajit/src/", 
			"msvcbuild.bat",
			[[copy lua51.dll "../../../Lib"]],
			[[copy lua51.lib "../../../Lib"]],
			[[copy lua51.exp "../../../Lib"]]
		}
		
		rebuildcommands {
			"cd " ..BDIR_SOURCE.."luajit/src/", 
			"msvcbuild.bat"
		}
		
		cleancommands {
			"cd " ..BDIR_SOURCE.."luajit/src/",
			"del *.dll",
			"del *.exe",
			"del *.lib"
		}
	
	configuration {}
	
	targetdir( "src/" )
	location( DIR_BUILD )
	
	buildoutputs { "lua51.lib", "lua51.dll" }
