
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB all_spritestest_src *.cpp *.hpp)
set(spritestest_src ${all_spritestest_src})
add_executable(spritestest ${spritestest_src})

# Link the executable 
target_link_libraries(spritestest fullcircle ${Boost_LIBRARIES} )

ADD_TEST(SpriteTest ${CMAKE_CURRENT_BINARY_DIR}/spritestest
  SpriteTest)

# add programs to the install target - do not install the hashbench benchmark program.
#INSTALL(PROGRAMS 
#  ${CMAKE_CURRENT_BINARY_DIR}/klio-add
#  DESTINATION bin)
#
