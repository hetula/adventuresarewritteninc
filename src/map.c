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
#include "simplexnoise/simplexnoise.h"
#include "adventures.h"
#include "tileobjectgenerator.h"

static const double PI_CONST = M_PI * 2;

uint8_t map_noise_to_terrain(double noise) {
    if (noise < .35) {
        return TERRAIN_FOREST;
    }
    if (noise < .5) {
        return TERRAIN_GRASS;
    }
    if (noise < .65) {
        return TERRAIN_FOREST;
    }
    if (noise < .7) {
        return TERRAIN_HILL;
    }
    if (noise < .78f) {
        return TERRAIN_MOUNTAIN;
    }
    return TERRAIN_SNOWY_MOUNTAIN;
}

uint8_t map_noise_to_water(double noise) {
    if (noise < .38) {
        return TERRAIN_LAKE;
    }
    if (noise < .42) {
        return TERRAIN_BEACH;
    }
    return 0;
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

double
gen_noise(SimplexNoiseContext *ctx, double x, double y, double w, double h, int feature_size) {

    double dx = feature_size;
    double dy = feature_size;
    double s = x / w;
    double t = y / h;
    double nx = cos(s * PI_CONST) * dx / PI_CONST;
    double ny = cos(t * PI_CONST) * dy / PI_CONST;
    double nz = sin(s * PI_CONST) * dx / PI_CONST;
    double nw = sin(t * PI_CONST) * dy / PI_CONST;

    return simplex_noise4(ctx, nx, ny, nz, nw);
}

void generate_map(Map *map, long seed, int save_data) {
    SimplexNoiseContext *baseCtx;
    simplex_noise_init(seed, &baseCtx);
    FILE *f = NULL;
    if (save_data == TRUE) {
        f = fopen("noise_data.txt", "w");
    }
    int w = map->width;
    int h = map->height;
    double noise;
    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int x = 0; x < w; x++) {
            noise = gen_noise(baseCtx, x, y, w, h, 72);
            map->data[x + y * w]->terrain_type = map_noise_to_terrain(noise);
            map->data[x + y * w]->y = y;
            map->data[x + y * w]->x = x;
            if (save_data == TRUE) {
                fprintf(f, "%f\n", noise);
            }
        }
    }
    simplex_noise_free(baseCtx);
    if (save_data == TRUE) {
        fprintf(f, "Water Noise\n");
    }

    SimplexNoiseContext *waterCtx;
    simplex_noise_init(seed / 2, &waterCtx);
    uint8_t terrain;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (can_map_water(map->data[x + y * w]->terrain_type) == FALSE) {
                continue;
            }
            noise = gen_noise(waterCtx, x, y, w, h, 144);
            terrain = map_noise_to_water(noise);
            if (save_data == TRUE) {
                fprintf(f, "%f\n", noise);
            }
            if (terrain != 0) {
                map->data[x + y * w]->terrain_type = terrain;

            }
        }
    }
    simplex_noise_free(waterCtx);
    generate_cities(map);
    if (save_data == TRUE) {
        fclose(f);
        save_map(map);
    }
    initialize_colors();
}

Terrain *get_terrain_at(int x, int y, const Player *player, const Map *map) {
    int realX = player->x + x;
    int realY = player->y + y;

    if (realX < 0) {
        realX = map->width + realX;
    }
    if (realY < 0) {
        realY = map->height + realY;
    }
    if (realX >= map->width) {
        realX = realX - map->width;
    }
    if (realY >= map->height) {
        realY = realY - map->height;
    }
    return get_terrain(map->data[realX + realY * map->width]->terrain_type);
}

const Terrain *terrain_at(const Map *map, int x, int y) {
    return get_terrain(map->data[x + y * map->width]->terrain_type);
}

void draw_map(const Map *map, const World *world, const Player *player) {
    int pX = MAP_WINDOW_WIDTH / 2;
    int pY = MAP_WINDOW_HEIGHT / 2;
    Terrain *terrain;
    int opt;
    // TODO: Make this as option
    //werase(map->win);
    //wrefresh(map->win);
    for (int y = -pY; y < pY; y++) {
        for (int x = -pX; x < pX; x++) {
            if (y == 0 && x == 0) {
                mvwprintw(map->win, pY + y, pX + x, "%c", '@');
            } else {
                terrain = get_terrain_at(x, y, player, map);
                opt = COLOR_PAIR(is_night(world->time) ? terrain->color_night : terrain->color_day);
                wattron(map->win, opt);
                mvwprintw(map->win, pY + y, pX + x, "%c", terrain->visual);
                wattroff(map->win, opt);
            }
            wrefresh(map->win);
        }
    }
}
