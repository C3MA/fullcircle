# -*- mode: cmake; -*-

file(GLOB fullcircle_hdrs *.h *.hpp)
install(FILES ${fullcircle_hdrs} DESTINATION include/libfullcircle)

configure_file(config.h.in ${CMAKE_BINARY_DIR}/libfullcircle/config.h)
install(FILES ${CMAKE_BINARY_DIR}/libfullcircle/config.h 
  DESTINATION include/libfullcircle)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Boost_INCLUDE_DIRS})
file(GLOB fullcircle_src 
  *.cpp *.hpp 
  sfx/*.cpp sfx/*.hpp
  printer/*.cpp printer/*.hpp
  net/*.cpp net/*.hpp
)
set(fullcircle_hdr simulator_main_window.hpp)
set(fullcircle_forms simulator.ui)
set(fullcircle_resources sprites.qrc)
QT4_WRAP_UI(fullcircle_hdr_forms ${fullcircle_forms})
QT4_WRAP_CPP(fullcircle_hdr_moc ${fullcircle_hdr} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
QT4_ADD_RESOURCES(fullcircle_rcc_resources ${fullcircle_resources})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS sequence.proto)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


ADD_LIBRARY(fullcircle STATIC 
  ${fullcircle_src} 
  ${PROTO_SRCS} ${PROTO_HDRS}
  ${fullcircle_hdr_moc}
  ${fullcircle_hdr_forms}
  ${fullcircle_rcc_resources}
  )

target_link_libraries(fullcircle
  ${Boost_LIBRARIES}
  ${PROTOBUF_LIBRARIES}
  ${QT_LIBRARIES}
	${OLA_LIBRARIES}
  pthread
)

#if(BUILD_SHARED_LIBS)
# install(TARGETS fullcircle LIBRARY DESTINATION lib)
  #else(BUILD_SHARED_LIBS)
   install(TARGETS fullcircle ARCHIVE DESTINATION lib)
  #endif(BUILD_SHARED_LIBS)

