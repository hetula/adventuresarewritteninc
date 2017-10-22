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
#include <math.h>
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

int initialize_colors() {
    init_pair(CLR_NIGHT_LAKE, 39, COLOR_BLACK);
    init_pair(CLR_NIGHT_BEACH, 11, COLOR_BLACK);
    init_pair(CLR_NIGHT_GRASS, 83, COLOR_BLACK);
    init_pair(CLR_NIGHT_FOREST, 22, COLOR_BLACK);
    init_pair(CLR_NIGHT_HILL, 94, COLOR_BLACK);
    init_pair(CLR_NIGHT_MOUNTAIN, 244, COLOR_BLACK);
    init_pair(CLR_NIGHT_SNOWY_MOUNTAIN, COLOR_WHITE, COLOR_BLACK);

    init_pair(CLR_DAY_LAKE, COLOR_WHITE, 39);
    init_pair(CLR_DAY_BEACH, COLOR_BLACK, 11);
    init_pair(CLR_DAY_GRASS, COLOR_BLACK, 83);
    init_pair(CLR_DAY_FOREST, COLOR_BLACK, 22);
    init_pair(CLR_DAY_HILL, COLOR_BLACK, 94);
    init_pair(CLR_DAY_MOUNTAIN, COLOR_BLACK, 244);
    init_pair(CLR_DAY_SNOWY_MOUNTAIN, COLOR_BLACK, COLOR_WHITE);
}

int map_noise_to_terrain(double noise) {
    if (noise < .3) {
        return TERRAIN_GRASS;
    }
    if (noise < .35) {
        return TERRAIN_FOREST;
    }
    if (noise < .5) {
        return TERRAIN_GRASS;
    }
    if (noise < .6) {
        return TERRAIN_FOREST;
    }
    if (noise < .7) {
        return TERRAIN_HILL;
    }
    if (noise < .75f) {
        return TERRAIN_MOUNTAIN;
    }
    return TERRAIN_SNOWY_MOUNTAIN;
}

int map_noise_to_water(double noise) {
    if (noise < .35) {
        return TERRAIN_LAKE;
    }
    if (noise < .37) {
        return TERRAIN_BEACH;
    }
    return -1;
}

double gen_octavenoise(struct SimplexNoiseContext *ctx, double x, double y,
                       int feature_size, int octaves, double roughness) {
    int oct;
    int octSum = 0;
    double noise = 0;
    for (int o = 0; o < octaves; o++) {
        oct = o == 0 ? 1 : o * 2;
        octSum += oct;
        noise += simplex_noise(ctx, x / feature_size / oct, y / feature_size / oct) * oct;
    }
    return pow(noise / octSum, roughness);
}

void generate_map(Map *map, long seed, int feature_size) {
    initialize_colors();
    struct SimplexNoiseContext *baseCtx;
    simplex_noise_init(seed, &baseCtx);
    int w = map->width;
    int h = map->height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            map->data[x + y * w] = map_noise_to_terrain(gen_octavenoise(baseCtx, x, y, feature_size, 6, .8));
        }
    }
    simplex_noise_free(baseCtx);

    struct SimplexNoiseContext *waterCtx;
    simplex_noise_init(seed / 2, &waterCtx);
    int terrain;
    int water_feat = feature_size * 2;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            terrain = map_noise_to_water(gen_octavenoise(waterCtx, x, y, water_feat, 6, .9));
            if (terrain != -1) {
                map->data[x + y * w] = terrain;
            }
        }
    }
    simplex_noise_free(waterCtx);
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
            return '~';
        case TERRAIN_BEACH:
            return '\'';
        case TERRAIN_GRASS:
            return '.';
        case TERRAIN_FOREST:
            return '#';
        case TERRAIN_HILL:
            return '^';
        case TERRAIN_MOUNTAIN:
            return '^';
        case TERRAIN_SNOWY_MOUNTAIN:
            return '^';
        default:
            return '?';
    }
}

void draw_map(const Map *map, const Player *player) {
    int w = map->width > 70 ? 70 : map->width;
    int h = map->height > 35 ? 35 : map->height;
    int pX = w / 2;
    int pY = h / 2;
    int *data = map->data;
    int terrain, opt;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (pX == x && pY == y) {
                mvaddch(y, x, '@');
            } else {
                terrain = data[(x + player->x) + (y + player->y) * map->width];
                opt = map_terrain_to_ui(terrain, FALSE);
                attron(opt);
                mvaddch(y, x, map_terrain_to_char(terrain));
                attroff(opt);
            }
            refresh();
        }
    }
}
