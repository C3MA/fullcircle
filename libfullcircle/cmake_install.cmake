# Install script for directory: /home/c3ma/Development/fullcircle/libfullcircle

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libfullcircle" TYPE FILE FILES
    "/home/c3ma/Development/fullcircle/libfullcircle/sequence.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/perlin_noise.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/common.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/color_scheme_plain.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/error.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/frame.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/sprite_io.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/simulator_main_window.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/color_scheme_smash.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/redis.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/scheduler.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/flowmap.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/color_scheme.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/fontrenderer.hpp"
    "/home/c3ma/Development/fullcircle/libfullcircle/dmxclient.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libfullcircle" TYPE FILE FILES "/home/c3ma/Development/fullcircle/libfullcircle/config.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/c3ma/Development/fullcircle/libfullcircle/libfullcircle.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

