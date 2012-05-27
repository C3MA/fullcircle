# -*- mode: cmake; -*-

file(GLOB fullcircle_hdrs *.h *.hpp)
install(FILES ${fullcircle_hdrs} DESTINATION include/libfullcircle)

configure_file(config.h.in ${CMAKE_BINARY_DIR}/libfullcircle/config.h)
install(FILES ${CMAKE_BINARY_DIR}/libfullcircle/config.h 
  DESTINATION include/libfullcircle)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB fullcircle_src *.cpp *.hpp)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS sequence.proto)

ADD_LIBRARY(fullcircle STATIC 
  ${fullcircle_src} 
  ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(fullcircle
  ${Boost_LIBRARIES}
  ${PROTOBUF_LIBRARIES}
  pthread
)

#if(BUILD_SHARED_LIBS)
# install(TARGETS fullcircle LIBRARY DESTINATION lib)
  #else(BUILD_SHARED_LIBS)
   install(TARGETS fullcircle ARCHIVE DESTINATION lib)
  #endif(BUILD_SHARED_LIBS)

