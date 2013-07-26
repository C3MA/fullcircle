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
include tests/sequences/CMakeFiles/sequencetest.dir/depend.make

# Include the progress variables for this target.
include tests/sequences/CMakeFiles/sequencetest.dir/progress.make

# Include the compile flags for this target's objects.
include tests/sequences/CMakeFiles/sequencetest.dir/flags.make

tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o: tests/sequences/CMakeFiles/sequencetest.dir/flags.make
tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o: tests/sequences/test_sequences.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/c3ma/Development/fullcircle/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o"
	cd /home/c3ma/Development/fullcircle/tests/sequences && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sequencetest.dir/test_sequences.cpp.o -c /home/c3ma/Development/fullcircle/tests/sequences/test_sequences.cpp

tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sequencetest.dir/test_sequences.cpp.i"
	cd /home/c3ma/Development/fullcircle/tests/sequences && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/c3ma/Development/fullcircle/tests/sequences/test_sequences.cpp > CMakeFiles/sequencetest.dir/test_sequences.cpp.i

tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sequencetest.dir/test_sequences.cpp.s"
	cd /home/c3ma/Development/fullcircle/tests/sequences && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/c3ma/Development/fullcircle/tests/sequences/test_sequences.cpp -o CMakeFiles/sequencetest.dir/test_sequences.cpp.s

tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.requires:
.PHONY : tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.requires

tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.provides: tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.requires
	$(MAKE) -f tests/sequences/CMakeFiles/sequencetest.dir/build.make tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.provides.build
.PHONY : tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.provides

tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.provides.build: tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o

# Object files for target sequencetest
sequencetest_OBJECTS = \
"CMakeFiles/sequencetest.dir/test_sequences.cpp.o"

# External object files for target sequencetest
sequencetest_EXTERNAL_OBJECTS =

tests/sequences/sequencetest: tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o
tests/sequences/sequencetest: libfullcircle/libfullcircle.a
tests/sequences/sequencetest: /usr/lib/libboost_test_exec_monitor-mt.a
tests/sequences/sequencetest: /usr/lib/libboost_program_options-mt.so
tests/sequences/sequencetest: /usr/lib/libboost_filesystem-mt.so
tests/sequences/sequencetest: /usr/lib/libboost_regex-mt.so
tests/sequences/sequencetest: /usr/lib/libboost_thread-mt.so
tests/sequences/sequencetest: /usr/lib/libboost_system-mt.so
tests/sequences/sequencetest: /usr/lib/libprotobuf.so
tests/sequences/sequencetest: /usr/lib/i386-linux-gnu/libQtGui.so
tests/sequences/sequencetest: /usr/lib/i386-linux-gnu/libQtCore.so
tests/sequences/sequencetest: /usr/local/lib/libola.a
tests/sequences/sequencetest: /usr/local/lib/libolacommon.a
tests/sequences/sequencetest: /usr/lib/libboost_system-mt.so
tests/sequences/sequencetest: tests/sequences/CMakeFiles/sequencetest.dir/build.make
tests/sequences/sequencetest: tests/sequences/CMakeFiles/sequencetest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sequencetest"
	cd /home/c3ma/Development/fullcircle/tests/sequences && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sequencetest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/sequences/CMakeFiles/sequencetest.dir/build: tests/sequences/sequencetest
.PHONY : tests/sequences/CMakeFiles/sequencetest.dir/build

tests/sequences/CMakeFiles/sequencetest.dir/requires: tests/sequences/CMakeFiles/sequencetest.dir/test_sequences.cpp.o.requires
.PHONY : tests/sequences/CMakeFiles/sequencetest.dir/requires

tests/sequences/CMakeFiles/sequencetest.dir/clean:
	cd /home/c3ma/Development/fullcircle/tests/sequences && $(CMAKE_COMMAND) -P CMakeFiles/sequencetest.dir/cmake_clean.cmake
.PHONY : tests/sequences/CMakeFiles/sequencetest.dir/clean

tests/sequences/CMakeFiles/sequencetest.dir/depend:
	cd /home/c3ma/Development/fullcircle && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/c3ma/Development/fullcircle /home/c3ma/Development/fullcircle/tests/sequences /home/c3ma/Development/fullcircle /home/c3ma/Development/fullcircle/tests/sequences /home/c3ma/Development/fullcircle/tests/sequences/CMakeFiles/sequencetest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/sequences/CMakeFiles/sequencetest.dir/depend
