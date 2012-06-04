
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_storetest_src *.cpp *.hpp)
set(fontstest_src ${all_storetest_src})
add_executable(fontstest ${fontstest_src})

# Link the executable 
target_link_libraries(fontstest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(FontsTest ${CMAKE_CURRENT_BINARY_DIR}/fontstest FontsTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
