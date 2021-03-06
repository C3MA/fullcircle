# -*- mode: cmake; -*-
project(libfullcircle)
cmake_minimum_required(VERSION "2.6" FATAL_ERROR)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

OPTION(ENABLE_LOGGING
  "Build the project with logging enabled"
  OFF)

# have the full monty in makefiles
set(CMAKE_VERBOSE_MAKEFILE true)

# use ctest
ENABLE_TESTING()

set(V_MAJOR 0)
set(V_MINOR 3)
set(V_PATCH 0)

# add a path where some libraries might be stored
set(CMAKE_ADDITIONAL_PATH "$ENV{CMAKE_ADDITIONAL_PATH}" CACHE PATH "Path where many locally installed libraries can be found")

###################################################################################################
# No user settings below
###################################################################################################

# Where are the additional libraries installed? Note: provide includes
# path here, subsequent checks will resolve everything else
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_ADDITIONAL_PATH}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_ADDITIONAL_PATH}/lib)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-non-virtual-dtor -g -fomit-frame-pointer")

set(CMAKE_CXX_FLAGS_EXPERIMENTAL "-O0 -g -ggdb -Wall -Werror -W -Wshadow")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused-variable")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused-parameter")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused-function")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wno-system-headers")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wno-non-virtual-dtor")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -pg -fprofile-generate ")
set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -fprofile-arcs -ftest-coverage")
# gprof and gcov support
set(CMAKE_CXX_FLAGS_PROFILE "-O0 -g -ggdb -W -Wreturn-type -Wno-shadow")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Wunused-variable -Wunused-parameter")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Wunused-function -Wunused")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Woverloaded-virtual -Wno-system-headers")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Wno-non-virtual-dtor -pg -fprofile-arcs")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -ftest-coverage")

# Releases are made with the release build. Optimize code and strip binaries.
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-non-virtual-dtor -Werror")

# always set these flags
set(CMAKE_CXX_FLAGS "-Wall ")

# We depend on boost >= 1.46.1 because of the updated filesystem
# library.
SET(Boost_ADDITIONAL_VERSIONS "1.46.1" "1.48.0" "1.48" "1.49" "1.50" "1.51")
FIND_PACKAGE(Boost 1.46.1 COMPONENTS 
  test_exec_monitor 
  program_options 
  filesystem 
  regex
  thread
  system)
INCLUDE(FindHiredis)
# find googles protocol buffers
INCLUDE(FindProtobuf)
IF (PB_FOUND)
  message(STATUS "Found Protocol Buffers: ${PB_PROTOC_CMD}")
  include_directories(${PB_INCLUDE_DIR})
  #link_libraries(${PB_LIBRARY})
ENDIF (PB_FOUND)

# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

INCLUDE(FindOLA)
IF (OLA_FOUND)
	message(STATUS "Found Ola: ${OLA_LIBRARIES}")
	include_directories(${OLA_INCLUDE_DIR})
ENDIF (OLA_FOUND)

include_directories(${CMAKE_BINARY_DIR})


# Check for some compiler stuff
include(CMakeBackwardCompatibilityCXX)

# Reflect the package structure
add_subdirectory(src)
add_subdirectory(libfullcircle)
add_subdirectory(tests)
add_subdirectory(bml2fcs)

# add some files to the installation target
INSTALL(FILES README.md LICENSE DESTINATION share/doc/libfullcircle)

# CPACK packaging
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_GENERATOR "STGZ;TGZ;DEB;RPM")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "fullcircle library")
SET(CPACK_PACKAGE_VENDOR "Mathias Dalheimer")
SET(CPACK_PACKAGE_CONTACT "md@gonium.net")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR ${V_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${V_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${V_PATCH})
SET(CPACK_STRIP_FILES "bin/fc-demo")
SET(CPACK_SOURCE_STRIP_FILES TRUE)
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_PACKAGE_EXECUTABLES "fc-demo" "Demo of the library")
SET(CPACK_PACKAGE_EXECUTABLES "fc-perlin" "Demo of the library")
SET(CPACK_PACKAGE_EXECUTABLES "fc-getid" "Demo of the library")
SET(CPACK_PACKAGE_EXECUTABLES "fc-badge" "Demo of the library")
INCLUDE(CPack)


### print some output for the user
message("")
message("***** Configuration *****")
message("  prefix: ${CMAKE_INSTALL_PREFIX}")
message("  boost include: ${Boost_INCLUDE_DIRS}")
message("  boost program options lib: ${Boost_PROGRAM_OPTIONS_LIBRARY}")
message("  boost system lib: ${Boost_SYSTEM_LIBRARY}")
message("  boost filesystem lib: ${Boost_FILESYSTEM_LIBRARY}")
message("  hiredis include: ${HIREDIS_INCLUDE_DIR}, lib: ${HIREDIS_LIBRARIES}")
message("  PB include: ${PROTOBUF_INCLUDE_DIRS}, lib: ${PROTOBUF_LIBRARIES}")
message("  QT4 include: ${QT_INCLUDE_DIR}, lib: ${QT_LIBRARIES}")
message("  OLA include: ${OLA_INCLUDE_DIR}, lib: ${OLA_LIBRARIES}")
message("  extended logging: ${ENABLE_LOGGING}")

