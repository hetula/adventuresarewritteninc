cmake_minimum_required(VERSION 3.8)
project(adventuresarewritteninc)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DISTRIBUTION "-Ofast")

set(CURSES_NEED_NCURSES true)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES src/adventures.c src/noise.c src/noise.h src/map.h src/map.c)
add_executable(adventuresarewritteninc ${SOURCE_FILES})
target_link_libraries(adventuresarewritteninc ${CURSES_LIBRARIES})