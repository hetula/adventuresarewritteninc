cmake_minimum_required(VERSION 3.8)
project(adventuresarewritteninc)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DISTRIBUTION "-Ofast")

set(CURSES_NEED_NCURSES true)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

set(SOURCE_FILES src/adventures.c src/map.h src/map.c src/imageutil.c src/imageutil.h src/simplexnoise.c src/simplexnoise.h src/player.c src/player.h src/adventures.h src/world.c src/world.h)
add_executable(adventuresarewritteninc ${SOURCE_FILES})
target_link_libraries(adventuresarewritteninc m)
target_link_libraries(adventuresarewritteninc ${CURSES_LIBRARIES})
target_link_libraries(adventuresarewritteninc ${PNG_LIBRARY})