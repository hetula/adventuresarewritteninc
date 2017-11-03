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
#ifndef ADVENTURESAREWRITTENINC_ADVENTURES_H
#define ADVENTURESAREWRITTENINC_ADVENTURES_H
/*
 * All Global Constants go here for easy configuration changes
 */

static const unsigned int MAP_WIDTH = 768;
static const unsigned int MAP_HEIGHT = 512;
static const unsigned long MAP_FULL_SIZE = MAP_WIDTH * MAP_HEIGHT; // 1 048 576 -> 393 216

static const int MAP_WINDOW_WIDTH = 48;
static const int MAP_WINDOW_HEIGHT = 24;

static const int WORLD_WINDOW_WIDTH = 32;
static const int WORLD_WINDOW_HEIGHT = 4;

static const int PLAYER_WINDOW_WIDTH = 32;
static const int PLAYER_WINDOW_HEIGHT = 16;

static const int MAX_BUILDINGS = 3000;
static const int MAX_METROPOLIES = 30;
static const int MAX_CITIES = 150;
static const int MAX_TOWNS = 800;

static const int FARM_MIN_DISTANCE = 8;
static const int TOWN_MIN_DISTANCE = 16;
static const int CITY_MIN_DISTANCE = 32;
static const int METROPOLIS_MIN_DISTANCE = 64;

#endif //ADVENTURESAREWRITTENINC_ADVENTURES_H
