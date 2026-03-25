cmake_minimum_required(VERSION 3.10...4.1)
project(ChurchullMagnum)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

add_subdirectory(extern/corrade EXCLUDE_FROM_ALL)

set(MAGNUM_WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)

add_subdirectory(extern/magnum EXCLUDE_FROM_ALL)
add_subdirectory(src)
