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
include src/CMakeFiles/fc-demo.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/fc-demo.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/fc-demo.dir/flags.make

src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o: src/CMakeFiles/fc-demo.dir/flags.make
src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o: src/fc-demo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/c3ma/Development/fullcircle/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o"
	cd /home/c3ma/Development/fullcircle/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fc-demo.dir/fc-demo.cpp.o -c /home/c3ma/Development/fullcircle/src/fc-demo.cpp

src/CMakeFiles/fc-demo.dir/fc-demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fc-demo.dir/fc-demo.cpp.i"
	cd /home/c3ma/Development/fullcircle/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/c3ma/Development/fullcircle/src/fc-demo.cpp > CMakeFiles/fc-demo.dir/fc-demo.cpp.i

src/CMakeFiles/fc-demo.dir/fc-demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fc-demo.dir/fc-demo.cpp.s"
	cd /home/c3ma/Development/fullcircle/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/c3ma/Development/fullcircle/src/fc-demo.cpp -o CMakeFiles/fc-demo.dir/fc-demo.cpp.s

src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.requires:
.PHONY : src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.requires

src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.provides: src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/fc-demo.dir/build.make src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.provides.build
.PHONY : src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.provides

src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.provides.build: src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o

# Object files for target fc-demo
fc__demo_OBJECTS = \
"CMakeFiles/fc-demo.dir/fc-demo.cpp.o"

# External object files for target fc-demo
fc__demo_EXTERNAL_OBJECTS =

src/fc-demo: src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o
src/fc-demo: libfullcircle/libfullcircle.a
src/fc-demo: /usr/lib/libboost_test_exec_monitor-mt.a
src/fc-demo: /usr/lib/libboost_program_options-mt.so
src/fc-demo: /usr/lib/libboost_filesystem-mt.so
src/fc-demo: /usr/lib/libboost_regex-mt.so
src/fc-demo: /usr/lib/libboost_thread-mt.so
src/fc-demo: /usr/lib/libboost_system-mt.so
src/fc-demo: /usr/lib/i386-linux-gnu/libQtGui.so
src/fc-demo: /usr/lib/i386-linux-gnu/libQtCore.so
src/fc-demo: /usr/lib/libprotobuf.so
src/fc-demo: /usr/local/lib/libola.a
src/fc-demo: /usr/local/lib/libolacommon.a
src/fc-demo: /usr/lib/libboost_system-mt.so
src/fc-demo: /usr/lib/i386-linux-gnu/libQtGui.so
src/fc-demo: /usr/lib/i386-linux-gnu/libQtCore.so
src/fc-demo: src/CMakeFiles/fc-demo.dir/build.make
src/fc-demo: src/CMakeFiles/fc-demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable fc-demo"
	cd /home/c3ma/Development/fullcircle/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fc-demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/fc-demo.dir/build: src/fc-demo
.PHONY : src/CMakeFiles/fc-demo.dir/build

src/CMakeFiles/fc-demo.dir/requires: src/CMakeFiles/fc-demo.dir/fc-demo.cpp.o.requires
.PHONY : src/CMakeFiles/fc-demo.dir/requires

src/CMakeFiles/fc-demo.dir/clean:
	cd /home/c3ma/Development/fullcircle/src && $(CMAKE_COMMAND) -P CMakeFiles/fc-demo.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/fc-demo.dir/clean

src/CMakeFiles/fc-demo.dir/depend:
	cd /home/c3ma/Development/fullcircle && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/c3ma/Development/fullcircle /home/c3ma/Development/fullcircle/src /home/c3ma/Development/fullcircle /home/c3ma/Development/fullcircle/src /home/c3ma/Development/fullcircle/src/CMakeFiles/fc-demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/fc-demo.dir/depend
