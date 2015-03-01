version = "2.1"
function sfmlproject(libname)
    project ("sfml-" .. libname)
		targetdir( DIR_LIB )
		location( DIR_BUILD )
		
        language "C++"
 
        kind ( "StaticLib" )
        defines { "SFML_STATIC" }
 
        configuration "Release"
			optimize "On"
			
		configuration "Debug"
			defines { "DEBUG=1", "_DEBUG=1" }
			flags { "Symbols" }
			optimize "Off"
			
		configuration "RelWithDebInfo"
			flags { "Symbols" }
			optimize "On"
 
        configuration "windows"
			flags { "WinMain" }
            defines { "WIN32", "_WIN32" }
 
        configuration { "windows", "SharedLib", "Native or x32" }
            libdirs { "extlibs/libs-msvc/x86" }
 
        configuration { "windows", "SharedLib", "x64" }
            libdirs { "extlibs/libs-msvc/x64" }
 
        configuration "macosx"
            defines { "MACOSX" }
 
        configuration "linux"
            defines { "LINUX" }
 
        -- must reset configuration selector so projects can add to global scope of project
        configuration {}
end
 
	--Commeneted out unused modules
	
    --[[sfmlproject "audio"
        files { "include/SFML/Audio/*.h", "src/SFML/Audio/*.cpp" }
        includedirs { "include", "src", "extlibs/headers/AL" }
 
        configuration "SharedLib"
            defines { "SFML_AUDIO_EXPORTS" }
            links { "sfml-system", "sndfile" }
 
        configuration "windows"
            includedirs { "extlibs/headers/libsndfile/windows" }
 
        configuration { "windows", "SharedLib" }
            links { "openal32" }
 
        configuration "macosx"
            includedirs { "extlibs/headers/libsndfile/osx" }
 
    sfmlproject "graphics"
        files { "src/SFML/Graphics/*.cpp" }
        includedirs { "include", "src", "extlibs/headers", "extlibs/headers/jpeg" }
        defines { "GLEW_STATIC", "STBI_FAILURE_USERMSG" }
 
        configuration "SharedLib"
            defines { "SFML_GRAPHICS_EXPORTS" }
            links { "sfml-system", "sfml-window" }
 
        configuration "windows"
            defines { "_CRT_SECURE_NO_WARNINGS" }
            includedirs { "extlibs/headers/libfreetype/windows" }
 
        configuration { "windows", "SharedLib" }
            links { "freetype", "glew", "jpeg", "opengl32" }
 
        configuration "macosx"
            includedirs { "extlibs/headers/libfreetype/osx" }]]
 
    sfmlproject "network"
        files { "src/SFML/Network/*.cpp" }
        includedirs { "include", "src" }
 
        configuration "SharedLib"
            defines { "SFML_NETWORK_EXPORTS" }
            links { "sfml-system" }
 
        configuration "windows"
            files { "src/SFML/Network/Win32/*.cpp" }
 
        configuration { "windows", "SharedLib" }
            links { "ws2_32" }
 
        configuration "macosx"
            files { "src/SFML/Network/Unix/*.cpp" }
 
        configuration "linux"
            files { "src/SFML/Network/Unix/*.cpp" }
 
    sfmlproject "system"
        files { "src/SFML/System/*.cpp" }
        includedirs { "include", "src" }
 
		defines { "UNICODE" }
 
        configuration "SharedLib"
            defines { "SFML_SYSTEM_EXPORTS" }
 
        configuration "windows"
            files { "src/SFML/System/Win32/*.cpp" }
 
        configuration "macosx"
            files { "src/SFML/System/Unix/*.cpp" }
 
        configuration "linux"
            files { "src/SFML/System/Unix/*.cpp" }
 
    --[[sfmlproject "window"
        files { "src/SFML/Window/*.cpp" }
        includedirs { "include", "src" }
        links { "sfml-system" }
		
		excludes { "*egl*.**" }
		
		defines { "UNICODE" }
 
        configuration "SharedLib"
            defines { "SFML_WINDOW_EXPORTS" }
            links { "sfml-system" }
 
        configuration "windows"
            files { "src/SFML/Window/Win32/*.cpp" }
 
        configuration { "windows", "SharedLib" }
            links { "opengl32", "winmm", "gdi32" }
 
        configuration "macosx"
            files { "src/SFML/Window/OSX/*.cpp" }
 
        configuration "linux"
            files { "src/SFML/Window/Linux/*.cpp" }]]
 
    -- not sfmlproject because it is only ever a static library
	-- sfmlproject is now all static, fuck you
    sfmlproject "main"
        files { "src/SFML/Main/*.cpp" }
        includedirs { "include", "src" }
 
        