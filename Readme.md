#Building 
To build this project, you need to have CMake installed on your computer.
If you do not have it, you can download it for free from http://www.cmake.org/
If you already have it, simply invoke it from here or from any other directory you prefer.
For example, you can write:

    mkdir build
    cd build
    cmake -G "Visual Studio 11" ..
	
Then you can build it from Visual Studio using command
    msbuild /m ALL_BUILD.vcxproj
or from GUI.

Of course, you can use any generator you wish that is supported by CMake.

#Troubleshooting during building
It is highly possible, that CMake will encounter problems during the run. Most often it is due to invalid environment configuration.
The following environment variables should be available:

* BOOST\_ROOT (or BOOST\_INCLUDEDIR and BOOST\_LIBRARYDIR) - pointing to Boost library path
* TURTLE\_ROOT - pointing to include directory in Turtle library directory (turtle is a header-only library, so it's kind of a root...)
* SHARK\_INCLUDE and SHARK\_LIBRARIES - pointing to Shark library includes and libs. Libs for release and debug should be located in one directory, however, release mode is not yet supported via CMake
if they are not available, CMake will complain about it. You can solve it in two ways:

	1. Easy - install these libraries and add required environment variables
	2. Clean - run CMake with -Dxxx command line arguments. Example:
    cmake -G "Visual Studio 11" .. -DSHARK\_INCLUDE=D:/Shark/include -DSHARK\_LIBRARIES=D:/Shark/libs -TURTLE\_ROOT=D:/turtle/include

	
#Detailed build steps

##Windows
It is assumed you have the following tools in your environment:

1. TortoiseSVN (available for free from http://tortoisesvn.net/)
2. Visual Studio 2013 or higher (Express Edition is sufficient - available for free from http://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx - registration is required, but free of charge, even for commercial use)
3. CMake 2.8.11 or higher

If you already have Boost, Shark or Turtle installed, you may skip steps connected with it.

1. Download Boost from http://www.boost.org/ . Unfortunately, version 1.55.0 cannot be compiled with Visual Studio straight away, so try using newer version if possible. You may also try 1.54.0, 
since application doesn't use any new features, but it also is not guaranteed to work.
1.1 If Boost 1.55.0 is still the latest version available, open file boost/archive/iterators/transform_width.hpp and add line 
    <code>#include &lt;algorithm&gt; </code>
in the beginning
2. Open Visual Studio Command Prompt and navigate to Boost root directory. Then type the following commands:

        bootstrap
	    b2 --layout=versioned
	
3. Wait until boost builds itself (in the meantime you can go to 5.)
4. Add BOOST\_ROOT to your environment variables and set it to Boost location (you may also add BOOST\_LIB/BOOST\_LIBRARYDIR, but it's optional)
5. Download Turtle from http://turtle.sourceforge.net/
6. Add TURTLE\_ROOT to your environment variables and set it to Turtle include location
7. Checkout Shark from https://svn.code.sf.net/p/shark-project/code/trunk/Shark. Use HEAD revision, or at least rev. 3028. You can do this by using TortoiseSVN GUI or by using command:

        svn co https://svn.code.sf.net/p/shark-project/code/trunk/Shark 
	
	in destination directory
	
8. Navigate to Shark root directory, then type the following commands:

        mkdir build
    	cd build
    	cmake -G "Visual Studio 12" ..
    	msbuild /m ALL_BUILD.vcxproj

9. Set SHARK\_INCLUDE environment variable to shark header path (usually <shark_root>/include)
10. Set SHARK\_LIBRARIES environment variable to shark library path (it is recommended to use <shark_root>/lib, but you may use whichever you like)
11. Go to application root
12. Type the following commands:

        mkdir build
    	cd build
    	cmake -G "Visual Studio 12" ..
    	msbuild /m ALL_BUILD.vcxproj
		
13. If everything went well, you should be able to run the application. Congratulations!    

#Executing
To execute application, run Recognizer.exe (on Windows). To check options use --help argument. 
Since this is a command line tool, don't expect any windows, only bare output.
Application can be controlled via command line arguments and via configuration file.
