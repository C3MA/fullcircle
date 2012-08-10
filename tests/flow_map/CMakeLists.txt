
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_flowmaptest_src *.cpp *.hpp)
set(flowmaptest_src ${all_flowmaptest_src})
add_executable(flowmaptest ${flowmaptest_src})

# Link the executable 
target_link_libraries(flowmaptest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(FlowMapTest ${CMAKE_CURRENT_BINARY_DIR}/flowmaptest
  FlowMapTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
