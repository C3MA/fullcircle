# -*- mode: cmake; -*-
set(fullcircle-demo_src fc-demo.cpp)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS})
add_executable(fc-demo ${fullcircle-demo_src})
# Link the executable 
target_link_libraries(fc-demo fullcircle ${Boost_LIBRARIES})

# sensor programs to the install target
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/fc-demo
  DESTINATION bin)

