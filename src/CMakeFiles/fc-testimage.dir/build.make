# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/c3ma/Development/fullcircle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/c3ma/Development/fullcircle

# Include any dependencies generated for this target.
include src/CMakeFiles/fc-testimage.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/fc-testimage.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/fc-testimage.dir/flags.make

src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o: src/CMakeFiles/fc-testimage.dir/flags.make
src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o: src/fc-testimage.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/c3ma/Development/fullcircle/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o"
	cd /home/c3ma/Development/fullcircle/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o -c /home/c3ma/Development/fullcircle/src/fc-testimage.cpp

src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fc-testimage.dir/fc-testimage.cpp.i"
	cd /home/c3ma/Development/fullcircle/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/c3ma/Development/fullcircle/src/fc-testimage.cpp > CMakeFiles/fc-testimage.dir/fc-testimage.cpp.i

src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fc-testimage.dir/fc-testimage.cpp.s"
	cd /home/c3ma/Development/fullcircle/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/c3ma/Development/fullcircle/src/fc-testimage.cpp -o CMakeFiles/fc-testimage.dir/fc-testimage.cpp.s

src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.requires:
.PHONY : src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.requires

src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.provides: src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/fc-testimage.dir/build.make src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.provides.build
.PHONY : src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.provides

src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.provides.build: src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o

# Object files for target fc-testimage
fc__testimage_OBJECTS = \
"CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o"

# External object files for target fc-testimage
fc__testimage_EXTERNAL_OBJECTS =

src/fc-testimage: src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o
src/fc-testimage: libfullcircle/libfullcircle.a
src/fc-testimage: /usr/lib/libboost_test_exec_monitor-mt.a
src/fc-testimage: /usr/lib/libboost_program_options-mt.so
src/fc-testimage: /usr/lib/libboost_filesystem-mt.so
src/fc-testimage: /usr/lib/libboost_regex-mt.so
src/fc-testimage: /usr/lib/libboost_thread-mt.so
src/fc-testimage: /usr/lib/libboost_system-mt.so
src/fc-testimage: /usr/lib/libprotobuf.so
src/fc-testimage: /usr/lib/i386-linux-gnu/libQtGui.so
src/fc-testimage: /usr/lib/i386-linux-gnu/libQtCore.so
src/fc-testimage: /usr/local/lib/libola.a
src/fc-testimage: /usr/local/lib/libolacommon.a
src/fc-testimage: /usr/lib/libboost_system-mt.so
src/fc-testimage: src/CMakeFiles/fc-testimage.dir/build.make
src/fc-testimage: src/CMakeFiles/fc-testimage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable fc-testimage"
	cd /home/c3ma/Development/fullcircle/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fc-testimage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/fc-testimage.dir/build: src/fc-testimage
.PHONY : src/CMakeFiles/fc-testimage.dir/build

src/CMakeFiles/fc-testimage.dir/requires: src/CMakeFiles/fc-testimage.dir/fc-testimage.cpp.o.requires
.PHONY : src/CMakeFiles/fc-testimage.dir/requires

src/CMakeFiles/fc-testimage.dir/clean:
	cd /home/c3ma/Development/fullcircle/src && $(CMAKE_COMMAND) -P CMakeFiles/fc-testimage.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/fc-testimage.dir/clean

src/CMakeFiles/fc-testimage.dir/depend:
	cd /home/c3ma/Development/fullcircle && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/c3ma/Development/fullcircle /home/c3ma/Development/fullcircle/src /home/c3ma/Development/fullcircle /home/c3ma/Development/fullcircle/src /home/c3ma/Development/fullcircle/src/CMakeFiles/fc-testimage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/fc-testimage.dir/depend

