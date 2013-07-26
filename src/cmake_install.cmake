# Install script for directory: /home/c3ma/Development/fullcircle/src

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
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/home/c3ma/Development/fullcircle/src/fc-simulator"
    "/home/c3ma/Development/fullcircle/src/fc-demo"
    "/home/c3ma/Development/fullcircle/src/fc-badge"
    "/home/c3ma/Development/fullcircle/src/fc-add"
    "/home/c3ma/Development/fullcircle/src/fc-text"
    "/home/c3ma/Development/fullcircle/src/fc-concat"
    "/home/c3ma/Development/fullcircle/src/fc-getid"
    "/home/c3ma/Development/fullcircle/src/fc-gol"
    "/home/c3ma/Development/fullcircle/src/fc-perlin"
    "/home/c3ma/Development/fullcircle/src/fc-leader"
    "/home/c3ma/Development/fullcircle/src/fc-read-image"
    "/home/c3ma/Development/fullcircle/src/fc-testimage"
    "/home/c3ma/Development/fullcircle/src/fc-testrow"
    "/home/c3ma/Development/fullcircle/src/fc-flowmap"
    "/home/c3ma/Development/fullcircle/src/fc-2gif"
    "/home/c3ma/Development/fullcircle/src/fc-ping"
    "/home/c3ma/Development/fullcircle/src/fc-streamer"
    "/home/c3ma/Development/fullcircle/src/fc-player"
    "/home/c3ma/Development/fullcircle/src/fc-snake"
    "/home/c3ma/Development/fullcircle/src/fc-convert2gif.sh"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

