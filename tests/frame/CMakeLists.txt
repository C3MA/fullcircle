
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_storetest_src *.cpp *.hpp)
set(fillframetest_src ${all_storetest_src})
add_executable(fillframetest ${fillframetest_src})

# Link the executable 
target_link_libraries(fillframetest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(FillFrameTest ${CMAKE_CURRENT_BINARY_DIR}/fillframetest FillFrameTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
