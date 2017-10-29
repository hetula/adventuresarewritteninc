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
#include "citygen.h"
#include "utils.h"

#define MAX_BUILDINGS 10000
#define MAX_METROPOLIES 100
#define MAX_CITIES 500
#define MAX_TOWNS 2500

#define FARM_MANHATTAN 8
#define TOWN_MANHATTAN 16
#define CITY_MANHATTAN 32
#define METROPOLIS_MANHATTAN 64

int get_terrain_min_distance(int terrain) {
    switch (terrain) {
        case TERRAIN_FARM:
            return FARM_MANHATTAN;
        case TERRAIN_TOWN:
            return TOWN_MANHATTAN;
        case TERRAIN_CITY:
            return CITY_MANHATTAN;
        case TERRAIN_METROPOLIS:
            return METROPOLIS_MANHATTAN;
        default:
            return 9999999;
    }
}

void generate_cities(Map *map) {
    int cities = MAX_BUILDINGS;
    int c = 0;
    City cityArr[cities];
    City *cur;
    City temp;
    int ok;
    cur = &cityArr[c];
    unsigned int gen_terrain_type = TERRAIN_METROPOLIS;
    int metropolis = MAX_METROPOLIES;
    int citys = MAX_CITIES;
    int towns = MAX_TOWNS;

    while (cities > 0) {
        ok = TRUE;
        cur->x = get_rnd(map->width);
        cur->y = get_rnd(map->height);
        cur->terrain_type = gen_terrain_type;

        if (map->data[cur->x + cur->y * map->width]->terrain_type == TERRAIN_LAKE) {
            continue;
        }

        for (int i = 0; i < c; i++) {
            if (i == c) {
                continue;
            }
            temp = cityArr[i];
            if (temp.x == cur->x && temp.y == cur->y) {
                ok = FALSE;
                break;
            }
            if (manhattan_distance(temp.x, temp.y, cur->x, cur->y) <=
                get_terrain_min_distance(cur->terrain_type)) {
                ok = FALSE;
                break;
            }
        }


        if (ok == TRUE) {
            mvprintw(9, 0, "Generating cities: %d             ", cities);
            refresh();
            c++;
            cities--;
            switch (gen_terrain_type) {
                case TERRAIN_METROPOLIS:
                    metropolis--;
                    if (metropolis == 0) {
                        gen_terrain_type = TERRAIN_CITY;
                    }
                    break;
                case TERRAIN_CITY:
                    citys--;
                    if (citys == 0) {
                        gen_terrain_type = TERRAIN_TOWN;
                    }
                    break;
                case TERRAIN_TOWN:
                    towns--;
                    if (towns == 0) {
                        gen_terrain_type = TERRAIN_FARM;
                    }
                    break;
                default:
                    break;
            }
            cur = &cityArr[c];
        }
    }

    for (int i = 0; i < MAX_BUILDINGS; i++) {
        temp = cityArr[i];
        map->data[temp.x + temp.y * map->width]->terrain_type = temp.terrain_type;
    }

}
