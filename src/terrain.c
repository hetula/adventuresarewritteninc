#include <ncurses.h>
#include "terrain.h"

const Terrain LAKE = {
        '~', /* Visual */
        CLR_DAY_LAKE,
        CLR_NIGHT_LAKE,
        1
};
const Terrain BEACH = {
        '\'',
        CLR_DAY_BEACH,
        CLR_NIGHT_BEACH,
        1
};
const Terrain GRASS = {
        '.',
        CLR_DAY_GRASS,
        CLR_NIGHT_GRASS,
        1
};
const Terrain FOREST = {
        '#',
        CLR_DAY_FOREST,
        CLR_NIGHT_FOREST,
        1
};
const Terrain HILL = {
        '^',
        CLR_DAY_HILL,
        CLR_NIGHT_HILL,
        1
};
const Terrain MOUNTAIN = {
        '^',
        CLR_DAY_MOUNTAIN,
        CLR_NIGHT_MOUNTAIN,
        1
};
const Terrain SNOWY_MOUNTAIN = {
        '^',
        CLR_DAY_SNOWY_MOUNTAIN,
        CLR_NIGHT_SNOWY_MOUNTAIN,
        1
};

void initialize_colors() {
    init_pair(CLR_NIGHT_LAKE, 39, 233);
    init_pair(CLR_NIGHT_BEACH, 11, 233);
    init_pair(CLR_NIGHT_GRASS, 83, 233);
    init_pair(CLR_NIGHT_FOREST, 22, 233);
    init_pair(CLR_NIGHT_HILL, 94, 233);
    init_pair(CLR_NIGHT_MOUNTAIN, 244, 233);
    init_pair(CLR_NIGHT_SNOWY_MOUNTAIN, COLOR_WHITE, 233);

    init_pair(CLR_DAY_LAKE, COLOR_WHITE, 39);
    init_pair(CLR_DAY_BEACH, COLOR_BLACK, 11);
    init_pair(CLR_DAY_GRASS, COLOR_BLACK, 83);
    init_pair(CLR_DAY_FOREST, COLOR_BLACK, 22);
    init_pair(CLR_DAY_HILL, COLOR_BLACK, 94);
    init_pair(CLR_DAY_MOUNTAIN, COLOR_BLACK, 244);
    init_pair(CLR_DAY_SNOWY_MOUNTAIN, COLOR_BLACK, COLOR_WHITE);
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
        default:
            return NULL;
    }
}
