cmake_minimum_required(VERSION 3.8)
project(adventuresarewritteninc)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DISTRIBUTION "-Ofast")

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

set(SOURCE_FILES src/adventures.c src/map.h src/map.c src/imageutil.c src/imageutil.h src/simplexnoise/simplexnoise.c src/simplexnoise/simplexnoise.h src/player.c src/player.h src/adventures.h src/world.c src/world.h src/terrain.c src/terrain.h src/charactercreator.h src/charactercreator.c src/intro.c src/intro.h src/utils.c src/utils.h src/astar/AStar.c src/astar/AStar.h src/tileobjectgenerator.c src/tileobjectgenerator.h src/namegenerator.c src/namegenerator.h)
add_executable(adventuresarewritteninc ${SOURCE_FILES})
target_link_libraries(adventuresarewritteninc m)
target_link_libraries(adventuresarewritteninc ${CURSES_LIBRARIES})
target_link_libraries(adventuresarewritteninc ${PNG_LIBRARY})