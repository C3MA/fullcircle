## libfullcircle - a library for fullcircle LED installations

## INSTALLATION
So far, the library has been successfully compiled on Ubuntu Linux
11.10, Debian Linux 6.0 and Mac OS 10.7 (Lion). The library depends on 
several libraries:

1. Boost version 1.48.0 or later
2. sqlite3
3. pkg-config

The library uses CMake for compile-time configuration. A Makefile is
provided which wraps the build infrastructure. A simple

    $ make

should suffice to compile the library if all dependencies are available.
Afterwards, you can use

    $ make install

to install the library. If you prefer proper packaging, you can also
build installation packages in several formats:

    $ make release

builds the package with release settings (i.e. full optimization, no
debug symbols) and places the created packages in the 'build'
subdirectory.

## BUGS 
The library was developed with great care, but we cannot guarantee that
no bugs are in the library. 

## LICENSE
This library is (c) Mathias Dalheimer and distributed under the terms of
the GPLv3.

## REFERENCES
