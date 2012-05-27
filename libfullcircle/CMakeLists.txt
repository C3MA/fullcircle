# -*- mode: cmake; -*-

file(GLOB fullcircle_hdrs *.h *.hpp)
install(FILES ${fullcircle_hdrs} DESTINATION include/libfullcircle)

configure_file(config.h.in ${CMAKE_BINARY_DIR}/libfullcircle/config.h)
install(FILES ${CMAKE_BINARY_DIR}/libfullcircle/config.h DESTINATION include/libfullcircle)


include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB fullcircle_src *.cpp *.hpp)
ADD_LIBRARY(fullcircle STATIC ${fullcircle_src} )

target_link_libraries(fullcircle
  ${Boost_LIBRARIES}
)

#if(BUILD_SHARED_LIBS)
# install(TARGETS fullcircle LIBRARY DESTINATION lib)
  #else(BUILD_SHARED_LIBS)
   install(TARGETS fullcircle ARCHIVE DESTINATION lib)
  #endif(BUILD_SHARED_LIBS)

