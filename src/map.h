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

#define TERRAIN_LAKE 0
#define TERRAIN_BEACH 1
#define TERRAIN_GRASS 2
#define TERRAIN_FOREST 3
#define TERRAIN_HILL 4
#define TERRAIN_MOUNTAIN 5
#define TERRAIN_SNOWY_MOUNTAIN 6

typedef struct Map_ {
    int width;
    int height;
    int *data;
} Map;

void generate_map(Map *map, int seed, int feature_size);

void draw_map(const Map *map);

#endif //ADVENTURESAREWRITTENINC_MAP_GEN_H