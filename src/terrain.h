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

#define CLR_DAY_LAKE 1
#define CLR_DAY_BEACH 2
#define CLR_DAY_GRASS 3
#define CLR_DAY_FOREST 4
#define CLR_DAY_HILL 5
#define CLR_DAY_MOUNTAIN 6
#define CLR_DAY_SNOWY_MOUNTAIN 7

#define CLR_NIGHT_LAKE 8
#define CLR_NIGHT_BEACH 9
#define CLR_NIGHT_GRASS 10
#define CLR_NIGHT_FOREST 11
#define CLR_NIGHT_HILL 12
#define CLR_NIGHT_MOUNTAIN 13
#define CLR_NIGHT_SNOWY_MOUNTAIN 14

#define TERRAIN_LAKE 0
#define TERRAIN_BEACH 1
#define TERRAIN_GRASS 2
#define TERRAIN_FOREST 3
#define TERRAIN_HILL 4
#define TERRAIN_MOUNTAIN 5
#define TERRAIN_SNOWY_MOUNTAIN 6

struct Terrain_ {
    char name[16];
    char visual;
    int color_day;
    int color_night;
    float roughness;
};

typedef const struct Terrain_ Terrain;

extern Terrain LAKE;
extern Terrain BEACH;
extern Terrain GRASS;
extern Terrain FOREST;
extern Terrain HILL;
extern Terrain MOUNTAIN;
extern Terrain SNOWY_MOUNTAIN;

void initialize_colors();

Terrain *get_terrain(int terrain);

#endif //ADVENTURESAREWRITTENINC_TERRAIN_H
