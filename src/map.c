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
#include "map.h"
#include "imageutil.h"
#include "simplexnoise.h"

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

int map_noise_to_terrain(double noise) {
    if (noise < .1) {
        return TERRAIN_LAKE;
    }
    if (noise < .15) {
        return TERRAIN_BEACH;
    }
    if (noise < .3) {
        return TERRAIN_GRASS;
    }
    if (noise < .35) {
        return TERRAIN_FOREST;
    }
    if (noise < .36) {
        return TERRAIN_LAKE;
    }
    if (noise < .45) {
        return TERRAIN_GRASS;
    }
    if (noise < .55) {
        return TERRAIN_HILL;
    }
    if (noise < .65) {
        return TERRAIN_FOREST;
    }
    if (noise < .75) {
        return TERRAIN_HILL;
    }
    if (noise < .95f) {
        return TERRAIN_MOUNTAIN;
    }
    return TERRAIN_SNOWY_MOUNTAIN;
}

int initialize_colors() {
    init_pair(CLR_NIGHT_LAKE, COLOR_BLUE, COLOR_BLACK);
    init_pair(CLR_NIGHT_BEACH, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CLR_NIGHT_GRASS, COLOR_GREEN, COLOR_BLACK);
    init_pair(CLR_NIGHT_FOREST, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CLR_NIGHT_HILL, COLOR_CYAN, COLOR_BLACK);
    init_pair(CLR_NIGHT_MOUNTAIN, COLOR_RED, COLOR_BLACK);
    init_pair(CLR_NIGHT_SNOWY_MOUNTAIN, COLOR_WHITE, COLOR_BLACK);

    init_pair(CLR_DAY_LAKE, COLOR_WHITE, COLOR_BLUE);
    init_pair(CLR_DAY_BEACH, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CLR_DAY_GRASS, COLOR_BLACK, COLOR_GREEN);
    init_pair(CLR_DAY_FOREST, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(CLR_DAY_HILL, COLOR_BLACK, COLOR_CYAN);
    init_pair(CLR_DAY_MOUNTAIN, COLOR_BLACK, COLOR_RED);
    init_pair(CLR_DAY_SNOWY_MOUNTAIN, COLOR_BLACK, COLOR_WHITE);
}

void generate_map(Map *map, int seed, int feature_size) {
    initialize_colors();
    struct SimplexNoiseContext *ctx;
    simplex_noise_init(seed, &ctx);
    int w = map->width;
    int h = map->height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double v0 = simplex_noise(ctx, (double) x / feature_size / 4, (double) y / feature_size / 4);
            double v1 = simplex_noise(ctx, (double) x / feature_size / 2, (double) y / feature_size / 2);
            double v2 = simplex_noise(ctx, (double) x / feature_size / 1, (double) y / feature_size / 1);
            double value = v0 * 4 / 7.0 + v1 * 2 / 7.0 + v2 * 1 / 7.0;
            map->data[x + y * w] = map_noise_to_terrain(value);
        }
    }
    simplex_noise_free(ctx);
    save_map(map);
}

int map_terrain_to_ui(int terrain, int nigth) {
    switch (terrain) {
        case TERRAIN_LAKE:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_LAKE : CLR_DAY_LAKE);
        case TERRAIN_BEACH:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_BEACH : CLR_DAY_BEACH);
        case TERRAIN_GRASS:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_GRASS : CLR_DAY_GRASS);
        case TERRAIN_FOREST:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_FOREST : CLR_DAY_FOREST);
        case TERRAIN_HILL:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_HILL : CLR_DAY_HILL);
        case TERRAIN_MOUNTAIN:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_MOUNTAIN : CLR_DAY_MOUNTAIN);
        case TERRAIN_SNOWY_MOUNTAIN:
            return COLOR_PAIR(nigth == TRUE ? CLR_NIGHT_SNOWY_MOUNTAIN : CLR_DAY_SNOWY_MOUNTAIN);
        default:
            return COLOR_PAIR(0);
    }
}

char map_terrain_to_char(int terrain) {
    switch (terrain) {
        case TERRAIN_LAKE:
            return 'L';
        case TERRAIN_BEACH:
            return 'B';
        case TERRAIN_GRASS:
            return 'G';
        case TERRAIN_FOREST:
            return 'F';
        case TERRAIN_HILL:
            return 'H';
        case TERRAIN_MOUNTAIN:
            return 'M';
        case TERRAIN_SNOWY_MOUNTAIN:
            return 'S';
        default:
            return '?';
    }
}

void draw_map(const Map *map) {
    clear();
    int w = map->width > 70 ? 70 : map->width;
    int h = map->height > 35 ? 35 : map->height;
    int *data = map->data;
    int terrain, opt;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            terrain = data[x + y * w];
            opt = map_terrain_to_ui(terrain, FALSE);
            attron(opt);
            mvaddch(y, x, map_terrain_to_char(terrain));
            attroff(opt);
            refresh();
        }
    }
}
