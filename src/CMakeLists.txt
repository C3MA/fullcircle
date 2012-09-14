# -*- mode: cmake; -*-
set(fc-simulator_src fc-simulator.cpp)
set(fc-demo_src fc-demo.cpp)
set(fc-perlin_src fc-perlin.cpp)
set(fc-gol_src fc-gol.cpp)
set(fc-text_src fc-text.cpp)
set(fc-badge_src fc-badge.cpp)
set(fc-getid_src fc-getid.cpp)
set(fc-add_src fc-add.cpp)
set(fc-cat_src fc-cat.cpp)
set(fc-concat_src fc-concat.cpp)
set(fc-read-image_src fc-read-image.cpp)
set(fc-flowmap_src fc-flowmap.cpp)
set(fc-concat_src fc-concat.cpp)
set(fc-read-image_src fc-read-image.cpp)
set(fc-leader_src fc-leader.cpp)
set(fc-testimage_src fc-testimage.cpp)
set(fc-testrow_src fc-testrow.cpp)
set(fc-server_src fc-server.cpp)
set(fc-ping_src fc-ping.cpp)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS})

add_executable(fc-simulator 
  ${fc-simulator_src}
  ${SAMPLE_MOC_SRCS}
  ${SAMPLE_RC_SRCS}
  ${SAMPLE_UI_HDRS} 
  )

add_executable(fc-demo
  ${fc-demo_src}
  )

add_executable(fc-perlin
  ${fc-perlin_src}
  )

add_executable(fc-gol
  ${fc-gol_src}
  )

add_executable(fc-text
  ${fc-text_src}
  )
add_executable(fc-add
  ${fc-add_src}
  )
add_executable(fc-concat
  ${fc-concat_src}
  )
add_executable(fc-read-image
  ${fc-read-image_src}
  )

add_executable(fc-badge
  ${fc-badge_src}
  )

add_executable(fc-flowmap
  ${fc-flowmap_src}
  )

add_executable(fc-getid
  ${fc-getid_src}
  )

add_executable(fc-leader
  ${fc-leader_src}
  )

add_executable(fc-testimage
  ${fc-testimage_src}
  )

add_executable(fc-testrow
  ${fc-testrow_src}
  )

add_executable(fc-server
  ${fc-server_src}
  )

add_executable(fc-ping
  ${fc-ping_src}
  )

target_link_libraries(fc-simulator 
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

target_link_libraries(fc-demo
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

target_link_libraries(fc-perlin
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

target_link_libraries(fc-gol
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

target_link_libraries(fc-text
  fullcircle
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-add
  fullcircle
  ${Boost_LIBRARIES}
  )
target_link_libraries(fc-read-image
  fullcircle
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-concat
  fullcircle
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-badge
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

target_link_libraries(fc-flowmap
  fullcircle 
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )


target_link_libraries(fc-getid
  fullcircle 
  ${Boost_LIBRARIES}
  ${HIREDIS_LIBRARIES}
  )

target_link_libraries(fc-leader
  fullcircle 
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-testimage
  fullcircle 
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-testrow
  fullcircle 
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-server
  fullcircle 
  ${Boost_LIBRARIES}
  )

target_link_libraries(fc-ping
  fullcircle 
  ${Boost_LIBRARIES}
  )

# sensor programs to the install target
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/fc-simulator
  ${CMAKE_CURRENT_BINARY_DIR}/fc-demo
  ${CMAKE_CURRENT_BINARY_DIR}/fc-badge
  ${CMAKE_CURRENT_BINARY_DIR}/fc-add
  ${CMAKE_CURRENT_BINARY_DIR}/fc-text
  ${CMAKE_CURRENT_BINARY_DIR}/fc-concat
  ${CMAKE_CURRENT_BINARY_DIR}/fc-getid
  ${CMAKE_CURRENT_BINARY_DIR}/fc-gol
  ${CMAKE_CURRENT_BINARY_DIR}/fc-perlin
  ${CMAKE_CURRENT_BINARY_DIR}/fc-leader
  ${CMAKE_CURRENT_BINARY_DIR}/fc-read-image
  ${CMAKE_CURRENT_BINARY_DIR}/fc-testimage
  ${CMAKE_CURRENT_BINARY_DIR}/fc-testrow
  ${CMAKE_CURRENT_BINARY_DIR}/fc-flowmap
  ${CMAKE_CURRENT_BINARY_DIR}/fc-ping
  DESTINATION bin)

