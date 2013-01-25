
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_nettest_src *.cpp *.hpp)
set(nettest_src ${all_nettest_src})
add_executable(nettest ${nettest_src})

# Link the executable 
target_link_libraries(nettest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(NetTest ${CMAKE_CURRENT_BINARY_DIR}/nettest NetTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
