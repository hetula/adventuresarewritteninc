/*
 * MIT License
 *
 * Copyright (c) 2017 Tuomo Heino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef ADVENTURESAREWRITTENINC_MAP_GEN_H
#define ADVENTURESAREWRITTENINC_MAP_GEN_H

#include <ncurses.h>
#include "player.h"
#include "world.h"
#include "terrain.h"
#include "adventures.h"

typedef struct MapTile_ {
    unsigned int terrain_type;
    unsigned int x;
    unsigned int y;
} MapTile;

typedef struct Map_ {
    unsigned int width;
    unsigned int height;
    WINDOW *win;
    MapTile **data;
    void *tile_data;
} Map;

void generate_map(Map *map, long seed, int save_data);

const Terrain *terrain_at(const Map *map, int x, int y);

void draw_map(const Map *map, const World *world, const Player *player);

#endif //ADVENTURESAREWRITTENINC_MAP_GEN_H