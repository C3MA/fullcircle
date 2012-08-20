include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_config_filestest_src *.cpp *.hpp)
set(config_filestest_src ${all_config_filestest_src})
add_executable(config_filestest ${config_filestest_src})

# Link the executable 
target_link_libraries(config_filestest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(ConfigTest ${CMAKE_CURRENT_BINARY_DIR}/config_filestest
  ConfigTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
