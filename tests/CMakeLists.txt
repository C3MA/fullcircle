# -*- mode: cmake; -*-

configure_file(testconfig.h.in ${CMAKE_BINARY_DIR}/testconfig.h)

add_subdirectory(sequences)
add_subdirectory(frame)
add_subdirectory(fonts)
add_subdirectory(sprites)
