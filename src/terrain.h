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
#ifndef ADVENTURESAREWRITTENINC_TERRAIN_H
#define ADVENTURESAREWRITTENINC_TERRAIN_H

#define MAX_TILE_OBJECT_NAME_LENGTH 32

#define COLOR_DAY_LAKE 1
#define COLOR_NIGHT_LAKE 2
#define COLOR_DAY_BEACH 3
#define COLOR_NIGHT_BEACH 4
#define COLOR_DAY_GRASS 5
#define COLOR_NIGHT_GRASS 6
#define COLOR_DAY_FOREST 7
#define COLOR_NIGHT_FOREST 8
#define COLOR_DAY_HILL 9
#define COLOR_NIGHT_HILL 10
#define COLOR_DAY_MOUNTAIN 11
#define COLOR_NIGHT_MOUNTAIN 12
#define COLOR_DAY_SNOWY_MOUNTAIN 13
#define COLOR_NIGHT_SNOWY_MOUNTAIN 14
#define COLOR_DAY_SWAMP 15
#define COLOR_NIGHT_SWAMP 16
#define COLOR_DAY_DESERT 17
#define COLOR_NIGHT_DESERT 18
#define COLOR_DAY_PLAINS 19
#define COLOR_NIGHT_PLAINS 20
#define COLOR_DAY_WASTELAND 21
#define COLOR_NIGHT_WASTELAND 22
#define COLOR_DAY_GRASS_HILL 23
#define COLOR_NIGHT_GRASS_HILL 24
#define COLOR_DAY_PINE_FOREST 25
#define COLOR_NIGHT_PINE_FOREST 26
#define COLOR_DAY_SNOW_FOREST 27
#define COLOR_NIGHT_SNOW_FOREST 28
#define COLOR_DAY_SNOW_PLAINS 29
#define COLOR_NIGHT_SNOW_PLAINS 30
#define COLOR_DAY_JUNGLE 31
#define COLOR_NIGHT_JUNGLE 32

#define COLOR_DAY_FARM 64
#define COLOR_NIGHT_FARM 65

#define COLOR_DAY_TOWN 66
#define COLOR_NIGHT_TOWN 67

#define COLOR_DAY_CITY 68
#define COLOR_NIGHT_CITY 69

#define COLOR_DAY_METROPOLIS 70
#define COLOR_NIGHT_METROPOLIS 71

#define TERRAIN_LAKE 1
#define TERRAIN_BEACH 2
#define TERRAIN_GRASS 3
#define TERRAIN_FOREST 4
#define TERRAIN_HILL 5
#define TERRAIN_MOUNTAIN 6
#define TERRAIN_SNOWY_MOUNTAIN 7
#define TERRAIN_SWAMP 8
#define TERRAIN_DESERT 9
#define TERRAIN_PLAINS 10
#define TERRAIN_WASTELAND 11
#define TERRAIN_GRASS_HILL 12
#define TERRAIN_PINE_FOREST 13
#define TERRAIN_SNOW_FOREST 14
#define TERRAIN_SNOW_PLAINS 15
#define TERRAIN_JUNGLE 16

#define TERRAIN_FARM 64
#define TERRAIN_TOWN 65
#define TERRAIN_CITY 66
#define TERRAIN_METROPOLIS 67

struct Terrain_ {
    int type;
    char name[16];
    char visual;
    int color_day;
    int color_night;
    float travel_cross_time;
};

typedef const struct Terrain_ Terrain;

typedef struct TileObject_ {
    unsigned int x;
    unsigned int y;
    unsigned int terrain_type;
    char name[MAX_TILE_OBJECT_NAME_LENGTH + 1];
} TileObject;

typedef struct Metropolis_ {
    TileObject tileObject;
} Metropolis;

typedef struct City_ {
    TileObject tileObject;
} City;

typedef struct Town_ {
    TileObject tileObject;
} Town;

typedef struct Farm_ {
    TileObject tileObject;
} Farm;

extern Terrain UNKNOWN;
extern Terrain LAKE;
extern Terrain BEACH;
extern Terrain GRASS;
extern Terrain FOREST;
extern Terrain HILL;
extern Terrain MOUNTAIN;
extern Terrain SNOWY_MOUNTAIN;
extern Terrain SWAMP;
extern Terrain DESERT;
extern Terrain PLAINS;
extern Terrain WASTELAND;
extern Terrain GRASS_HILL;
extern Terrain PINE_FOREST;
extern Terrain SNOW_FOREST;
extern Terrain SNOW_PLAINS;
extern Terrain JUNGLE;

extern Terrain FARM;
extern Terrain TOWN;
extern Terrain CITY;
extern Terrain METROPOLIS;

void initialize_colors();

int is_terrain_town(int terrain);

Terrain *get_terrain(int terrain);

#endif //ADVENTURESAREWRITTENINC_TERRAIN_H
