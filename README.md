tcltaglib
=====
[Taglib](https://taglib.github.io/) is a library for reading and editing 
the meta-data of several popular audio formats. Currently it supports both 
ID3v1 and ID3v2 for MP3 files, Ogg Vorbis comments and ID3 tags and Vorbis 
comments in FLAC, MPC, Speex, WavPack TrueAudio, WAV, AIFF, MP4 and ASF files.

This extension is Tcl interface for taglib (Abstract API only).


Commands
=====

Implement tcl commands:  
taglib::file_new  
taglib::file_new_type  
taglib::file_is_valid  
taglib::file_tag  
taglib::file_save  
taglib::file_free  
taglib::audioproperties  
taglib::tag_free  
taglib::tag_title  
taglib::tag_artist  
taglib::tag_album  
taglib::tag_comment  
taglib::tag_genre  
taglib::tag_year  
taglib::tag_track  
taglib::set_title  
taglib::set_artist  
taglib::set_album  
taglib::set_comment  
taglib::set_genre  
taglib::set_year  
taglib::set_track  


UNIX BUILD
=====

I only test build tcltaglib under openSUSE LEAP 42.2 and Ubuntu 14.04.

Build Linux version please install development files for taglib first.
Below is an example for openSUSE:

    sudo zypper in libtag-devel

Below is an example for Ubuntu:

    sudo apt-get install libtagc0-dev

Building under most UNIX systems is easy, just run the configure script
and then run make. For more information about the build process, see the
tcl/unix/README file in the Tcl src dist. The following minimal example
will install the extension in the /opt/tcl directory.

	$ cd tcltaglib
	$ ./configure --prefix=/opt/tcl
	$ make
	$ make install

If you need setup directory containing tcl configuration (tclConfig.sh),
below is an example:

	$ cd tcltaglib
	$ ./configure --with-tcl=/opt/activetcl/lib
	$ make
	$ make install 

WINDOWS BUILD
=====

## MSYS2/MinGW-W64

Install CMake for MSYS2 first:

	pacman -S mingw-w64-x86_64-cmake

Build zlib and install static lib:

	make -fwin32/Makefile.gcc

	export INCLUDE_PATH=/c/msys64/mingw64/include
	export LIBRARY_PATH=/c/msys64/mingw64/lib
	export BINARY_PATH=/c/msys64/mingw64/bin
	make install -fwin32/Makefile.gcc SHARED_MODE=0

Build taglib and install:

	cmake -G "MSYS Makefiles" -DCMAKE_CXX_COMPILER="/mingw64/bin/g++.exe" \
	  -DCMAKE_MAKE_PROGRAM=/mingw64/bin/mingw32-make.exe \
	  -DZLIB_INCLUDE_DIR=/c/msys64/src/zlib-1.2.8 \
	  -DZLIB_LIBRARY=/c/msys64/src/zlib-1.2.8/libz.a \
	  -DCMAKE_INSTALL_PREFIX=/c/msys64/mingw64 \
	  -DCMAKE_BUILD_TYPE=Release -DWITH_ASF=ON -DWITH_MP4=ON \
	  -DBUILD_EXAMPLES=ON -DBUILD_SHARED_LIBS=ON .

	make
	make install

Finally, build tcltaglib and install:
	
	./configure --with-tcl=/c/tcl/lib
	make
	make install

User needs to put tag_c.dll and tag.dll to c:\Windows (or your windows folder or avaiable folder). Please also notice, taglib also needs libgcc_s_seh-1.dll, libstdc++-6.dll and libwinpthread-1.dll.


Examples
=====

Please check the examples folder.
