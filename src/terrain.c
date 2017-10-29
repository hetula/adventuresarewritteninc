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
#include <ncurses.h>
#include "terrain.h"

const Terrain UNKNOWN = {
        "UNKNOWN\0",
        '?', /* Visual */
        COLOR_DAY_LAKE,
        COLOR_NIGHT_LAKE,
        999
};

const Terrain LAKE = {
        "Lake\0",
        '~', /* Visual */
        COLOR_DAY_LAKE,
        COLOR_NIGHT_LAKE,
        120
};
const Terrain BEACH = {
        "Beach\0",
        '\'',
        COLOR_DAY_BEACH,
        COLOR_NIGHT_BEACH,
        45
};
const Terrain GRASS = {
        "Grass\0",
        '.',
        COLOR_DAY_GRASS,
        COLOR_NIGHT_GRASS,
        45
};
const Terrain FOREST = {
        "Forest\0",
        '#',
        COLOR_DAY_FOREST,
        COLOR_NIGHT_FOREST,
        90
};
const Terrain HILL = {
        "Hill\0",
        '^',
        COLOR_DAY_HILL,
        COLOR_NIGHT_HILL,
        102
};
const Terrain MOUNTAIN = {
        "Mountain\0",
        '^',
        COLOR_DAY_MOUNTAIN,
        COLOR_NIGHT_MOUNTAIN,
        480
};
const Terrain SNOWY_MOUNTAIN = {
        "Mountain Peak\0",
        '^',
        COLOR_DAY_SNOWY_MOUNTAIN,
        COLOR_NIGHT_SNOWY_MOUNTAIN,
        640
};

const Terrain FARM = {
        "Farm\0",
        'F',
        COLOR_DAY_FARM,
        COLOR_NIGHT_FARM,
        15
};

const Terrain TOWN = {
        "Town\0",
        'T',
        COLOR_DAY_TOWN,
        COLOR_NIGHT_TOWN,
        15
};

const Terrain CITY = {
        "City\0",
        'C',
        COLOR_DAY_CITY,
        COLOR_NIGHT_CITY,
        15
};

const Terrain METROPOLIS = {
        "Metropolis\0",
        'M',
        COLOR_DAY_METROPOLIS,
        COLOR_NIGHT_METROPOLIS,
        15
};

void initialize_colors() {
    init_pair(COLOR_NIGHT_LAKE, 39, 233);
    init_pair(COLOR_NIGHT_BEACH, 11, 233);
    init_pair(COLOR_NIGHT_GRASS, 83, 233);
    init_pair(COLOR_NIGHT_FOREST, 22, 233);
    init_pair(COLOR_NIGHT_HILL, 94, 233);
    init_pair(COLOR_NIGHT_MOUNTAIN, 244, 233);
    init_pair(COLOR_NIGHT_SNOWY_MOUNTAIN, COLOR_WHITE, 233);

    init_pair(COLOR_NIGHT_FARM, 94, 238);
    init_pair(COLOR_NIGHT_TOWN, 3, 238);
    init_pair(COLOR_NIGHT_CITY, 3, 238);
    init_pair(COLOR_NIGHT_METROPOLIS, 3, 238);

    init_pair(COLOR_DAY_LAKE, COLOR_WHITE, 39);
    init_pair(COLOR_DAY_BEACH, COLOR_BLACK, 11);
    init_pair(COLOR_DAY_GRASS, COLOR_BLACK, 83);
    init_pair(COLOR_DAY_FOREST, COLOR_BLACK, 22);
    init_pair(COLOR_DAY_HILL, COLOR_BLACK, 94);
    init_pair(COLOR_DAY_MOUNTAIN, COLOR_BLACK, 244);
    init_pair(COLOR_DAY_SNOWY_MOUNTAIN, COLOR_BLACK, COLOR_WHITE);

    init_pair(COLOR_DAY_FARM, 233, 94);
    init_pair(COLOR_DAY_TOWN, 233, 3);
    init_pair(COLOR_DAY_CITY, 233, 3);
    init_pair(COLOR_DAY_METROPOLIS, 233, 3);

}

int is_terrain_town(int terrain) {
    switch (terrain) {
        case TERRAIN_FARM:
        case TERRAIN_TOWN:
        case TERRAIN_CITY:
        case TERRAIN_METROPOLIS:
            return TRUE;
        default:
            return FALSE;
    }
}

Terrain *get_terrain(int terrain) {
    switch (terrain) {
        case TERRAIN_LAKE:
            return &LAKE;
        case TERRAIN_BEACH:
            return &BEACH;
        case TERRAIN_GRASS:
            return &GRASS;
        case TERRAIN_FOREST:
            return &FOREST;
        case TERRAIN_HILL:
            return &HILL;
        case TERRAIN_MOUNTAIN:
            return &MOUNTAIN;
        case TERRAIN_SNOWY_MOUNTAIN:
            return &SNOWY_MOUNTAIN;
        case TERRAIN_FARM:
            return &FARM;
        case TERRAIN_TOWN:
            return &TOWN;
        case TERRAIN_CITY:
            return &CITY;
        case TERRAIN_METROPOLIS:
            return &METROPOLIS;
        default:
            return &UNKNOWN;
    }
}
