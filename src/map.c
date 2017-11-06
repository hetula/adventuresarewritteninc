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

unsigned int map_noise_to_terrain(double base_noise) {
    if (base_noise < .35) {
        return TERRAIN_FOREST;
    }
    if (base_noise < .5) {
        return TERRAIN_GRASS;
    }
    if (base_noise < .6) {
        return TERRAIN_FOREST;
    }
    if (base_noise < .65) {
        return TERRAIN_HILL;
    }
    if (base_noise < .75f) {
        return TERRAIN_MOUNTAIN;
    }
    return TERRAIN_SNOWY_MOUNTAIN;
}

unsigned int map_noise_to_water(double water_noise) {
    if (water_noise < .4) {
        return TERRAIN_LAKE;
    }
    return 0;
}

unsigned int blend_noise_as_moisture(double moisture_noise, unsigned int old_terrain_type) {
    switch (old_terrain_type) {
        case TERRAIN_GRASS:
            if (moisture_noise < .25) {
                return TERRAIN_DESERT;
            }
            if (moisture_noise < .45) {
                return TERRAIN_PLAINS;
            }
            if (moisture_noise > .65) {
                return TERRAIN_SWAMP;
            }
            break;
        case TERRAIN_HILL:
            if (moisture_noise > .6) {
                return TERRAIN_GRASS_HILL;
            }
            break;
        default:
            return 0;
    }
    return 0;
}

unsigned int blend_noise_as_temperature(double temperature_noise, unsigned int old_terrain_type) {
    switch (old_terrain_type) {
        case TERRAIN_GRASS:
            if (temperature_noise < .3) {
                return TERRAIN_SNOW_PLAINS;
            }
            if (temperature_noise > .7) {
                return TERRAIN_DESERT;
            }
            break;
        case TERRAIN_PLAINS:
            if (temperature_noise < .3) {
                return TERRAIN_SNOW_PLAINS;
            }
            if (temperature_noise < .4) {
                return TERRAIN_WASTELAND;
            }
            if (temperature_noise > .65) {
                return TERRAIN_DESERT;
            }
            break;
        case TERRAIN_FOREST:
            if (temperature_noise < .3) {
                return TERRAIN_SNOW_FOREST;
            }
            if (temperature_noise < .4) {
                return TERRAIN_PINE_FOREST;
            }
            if (temperature_noise > .6) {
                return TERRAIN_JUNGLE;
            }
            break;
        case TERRAIN_HILL:
            if (temperature_noise > .5) {
                return TERRAIN_GRASS_HILL;
            }
            if (temperature_noise > .65) {
                return TERRAIN_JUNGLE;
            }
            break;
        default:
            return 0;
    }
    return 0;
}


int can_map_water(int terrain) {
    switch (terrain) {
        case TERRAIN_GRASS:
        case TERRAIN_PLAINS:
            return TRUE;
        default:
            return FALSE;
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

void gen_base_land(Map *map, long seed, int w, int h) {
    double noise;
    double min = 1;
    double max = 0;
    double mid = 0;

    SimplexNoiseContext *baseCtx;
    simplex_noise_init(seed, &baseCtx);
    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int x = 0; x < w; x++) {
            noise = gen_noise(baseCtx, x, y, w, h, BASE_SIZE);
            map->data[x + y * w]->terrain_type = map_noise_to_terrain(noise);
            map->data[x + y * w]->y = y;
            map->data[x + y * w]->x = x;
            mid += noise;
            if (noise < min) {
                min = noise;
            }
            if (noise > max) {
                max = noise;
            }
        }
    }
    simplex_noise_free(baseCtx);
    mid = mid / (w * h);
    if (PRINT_INFO == TRUE) {
        FILE *f = NULL;
        f = fopen("dbg_data.txt", "w");
        fprintf(f, "Base:\nMin:%f\nMid:%f\nMax:%f\n\n", min, mid, max);
        fclose(f);
    }
}

void gen_moisture(Map *map, long seed, int w, int h) {
    double noise;
    double min = 1;
    double max = 0;
    double mid = 0;
    unsigned int terrain;

    SimplexNoiseContext *moistureCtx;
    simplex_noise_init(seed / 4, &moistureCtx);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            noise = gen_noise(moistureCtx, x, y, w, h, MOISTURE_SIZE);
            terrain = blend_noise_as_moisture(noise, map->data[x + y * w]->terrain_type);
            if (terrain != 0) {
                map->data[x + y * w]->terrain_type = terrain;

            }
            mid += noise;
            if (noise < min) {
                min = noise;
            }
            if (noise > max) {
                max = noise;
            }
        }
    }
    simplex_noise_free(moistureCtx);
    mid = mid / (w * h);
    if (PRINT_INFO == TRUE) {
        FILE *f = NULL;
        f = fopen("dbg_data.txt", "a");
        fprintf(f, "Moisture:\nMin:%f\nMid:%f\nMax:%f\n\n", min, mid, max);
        fclose(f);
    }
}

void gen_temperature(Map *map, long seed, int w, int h) {
    double noise;
    double min = 1;
    double max = 0;
    double mid = 0;
    unsigned int terrain;

    SimplexNoiseContext *temperatureCtx;
    simplex_noise_init(seed / 8, &temperatureCtx);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (map->data[x + y * w]->terrain_type == TERRAIN_LAKE) {
                continue;
            }
            noise = gen_noise(temperatureCtx, x, y, w, h, TEMPERATURE_SIZE);
            terrain = blend_noise_as_temperature(noise, map->data[x + y * w]->terrain_type);
            if (terrain != 0) {
                map->data[x + y * w]->terrain_type = terrain;

            }
            mid += noise;
            if (noise < min) {
                min = noise;
            }
            if (noise > max) {
                max = noise;
            }
        }
    }
    simplex_noise_free(temperatureCtx);
    mid = mid / (w * h);
    if (PRINT_INFO == TRUE) {
        FILE *f = NULL;
        f = fopen("dbg_data.txt", "a");
        fprintf(f, "Temperature:\nMin:%f\nMid:%f\nMax:%f\n\n", min, mid, max);
        fclose(f);
    }
}

void gen_water(Map *map, long seed, int w, int h) {
    double noise;
    double min = 1;
    double max = 0;
    double mid = 0;
    unsigned int terrain;

    SimplexNoiseContext *waterCtx;
    simplex_noise_init(seed / 2, &waterCtx);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (can_map_water(map->data[x + y * w]->terrain_type) == FALSE) {
                continue;
            }
            noise = gen_noise(waterCtx, x, y, w, h, WATER_SIZE);
            terrain = map_noise_to_water(noise);
            if (terrain != 0) {
                map->data[x + y * w]->terrain_type = terrain;

            }
            mid += noise;
            if (noise < min) {
                min = noise;
            }
            if (noise > max) {
                max = noise;
            }
        }
    }
    simplex_noise_free(waterCtx);
    mid = mid / (w * h);
    if (PRINT_INFO == TRUE) {
        FILE *f = NULL;
        f = fopen("dbg_data.txt", "a");
        fprintf(f, "Water:\nMin:%f\nMid:%f\nMax:%f\n\n", min, mid, max);
        fclose(f);
    }
}

void generate_map(Map *map, long seed, int save_data) {
    int width = map->width;
    int height = map->height;

    gen_base_land(map, seed, width, height);
    gen_moisture(map, seed, width, height);
    gen_water(map, seed, width, height);
    gen_temperature(map, seed, width, height);

    generate_cities(map);
    if (save_data == TRUE) {
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
