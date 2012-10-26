# -*- mode: cmake; -*-
PROJECT(fc-bml2fcs)
FIND_PACKAGE(Qt4 REQUIRED)

set(fc-bml2fcs_src fc-bml2fcs.cpp SaxHandler.cpp)

set(QT_USE_QTXML TRUE)
set(QT_DONT_USE_GUI TRUE)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS})

add_executable(fc-bml2fcs
  ${fc-bml2fcs_src}
  )

target_link_libraries(fc-bml2fcs 
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

# sensor programs to the install target
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/fc-bml2fcs
  DESTINATION bin)

