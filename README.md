# SimpleHTTPServer #

A small library developed for use in applications to provide output via a webpage.

Made with extensibility and readability in mind. Makes extensive use of C++11.

The provided example will attempt to retrieve the requested file, if it is a lua file, it will process the file and return the output as the webpage. See `Data/index.lua`.

Lacks support for HTTP1.1 and websockets, only supports GET requests, but it can easily be tailored to specific needs.

Only tested on Windows, Linux may work with some configuration. (Uses SFML-Network, the premake script may need changes to compile on linux)

# Installation #

### Compilation ###

* **Windows - Visual Studio**: Run `premake-vs2013.bat`, a VS2013 solution will be generated in the **Build** directory, open up `ALL_BUILD.sln` and compile.
* **Command line**: Alternatively, you can navigate to the **Premake** directory, and use commands `premake gmake` or `premake.exe vs2010` for Windows. This has the provided benfit of command line options.
* **Clean**: Using the `premake-clean.sh` you can remove all temporary files.
* **Clean-all**: Using the `premake-clean-all.sh` you can remove all temporary and compiled binary files.

##### Premake actions #####

The action in premake is the task carried out, it is passed to premake as the first argument: `premake5.exe <action>`

* `clean` Removes all temporary files, removes only temporary files such as build objects. Used by 'premake-clean.sh'
* `clean-all` Sterilizes the repo to it's original state, removes all files including compiled binaries. Used by 'premake-clean-all.sh'
* `version` Prints out the version of the sources.
* `gen-docs` Attempts to create documentation using Doxygen (thereby it should be installed on the system beforehand), output in **Bin/Docs**.
* `gmake` Generates GNU Makefile build scripts\*
* `vs2005` Generate project files for VS2005\*
* `vs2008` Generate project files for VS2008\*
* `vs2010` Generate project files for VS2010\*
* `vs2012` Generate project files for VS2012\*
* `vs2013` Generate project files for VS2013. Used by 'premake-vs2013.bat'

\* Not supported


### Installation ###

After compiling, you probably need to install the libraries (.dll on windows and .so on others), to do this just use the provided `install-libraries.bat`.

#### Running the example program ####

Compile all projects in visual studio, run `install-libraries.bat` as above, go to `Bin` folder and run LuaServer.exe.
By default the program runs on the 9398 port, so to view the webpage, go to http://localhost:9398

# Contact #

* E-mail - [kamil.zmich@gmail.com](mailto:kamil.zmich@gmail.com)

# License #

SimpleHTTPServer is licensed under the MIT License