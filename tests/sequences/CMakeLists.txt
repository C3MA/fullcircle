
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_sequencetest_src *.cpp *.hpp)
set(sequencetest_src ${all_sequencetest_src})
add_executable(sequencetest ${sequencetest_src})

# Link the executable 
target_link_libraries(sequencetest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(SequenceTest ${CMAKE_CURRENT_BINARY_DIR}/sequencetest
  SequenceTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
