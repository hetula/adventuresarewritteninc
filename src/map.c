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
#include "adventures.h"

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

int can_map_water(int terrain) {
    switch (terrain) {
        case TERRAIN_SNOWY_MOUNTAIN:
        case TERRAIN_MOUNTAIN:
        case TERRAIN_HILL:
            return FALSE;
        default:
            return TRUE;
    }
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
            if (can_map_water(map->data[x + y * w]) == FALSE) {
                continue;
            }
            terrain = map_noise_to_water(gen_octavenoise(waterCtx, x, y, water_feat, 6, .9));
            if (terrain != -1) {
                map->data[x + y * w] = terrain;
            }
        }
    }
    simplex_noise_free(waterCtx);
    save_map(map);
}

void draw_map(const Map *map, const World *world, const Player *player) {
    int w = map->width > MAP_WINDOW_WIDTH ? MAP_WINDOW_WIDTH : map->width;
    int h = map->height > MAP_WINDOW_HEIGHT ? MAP_WINDOW_HEIGHT : map->height;
    int pX = w / 2;
    int pY = h / 2;
    int *data = map->data;
    Terrain *terrain;
    int opt;
    werase(map->win);
    wrefresh(map->win);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (pX == x && pY == y) {
                mvwprintw(map->win, y, x, "%c", '@');
            } else {
                terrain = get_terrain(data[(x + player->x) + (y + player->y) * map->width]);
                opt = COLOR_PAIR(is_night(world->time) ? terrain->color_night : terrain->color_day);
                wattron(map->win, opt);
                mvwprintw(map->win, y, x, "%c", terrain->visual);
                wattroff(map->win, opt);
            }
            wrefresh(map->win);
        }
    }
}
