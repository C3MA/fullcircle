# -*- mode: cmake; -*-
set(fc-simulator_src fc-simulator.cpp)
set(fc-demo_src fc-demo.cpp)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS})

add_executable(fc-simulator 
  ${fc-simulator_src}
  ${SAMPLE_MOC_SRCS}
  ${SAMPLE_RC_SRCS}
  ${SAMPLE_UI_HDRS} 
  )

add_executable(fc-demo
  ${fc-demo_src}
  )


target_link_libraries(fc-simulator 
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

target_link_libraries(fc-demo
  fullcircle 
  ${Boost_LIBRARIES}
  )

# sensor programs to the install target
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/fc-simulator
  ${CMAKE_CURRENT_BINARY_DIR}/fc-demo
  DESTINATION bin)

