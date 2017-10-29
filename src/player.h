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
#ifndef ADVENTURESAREWRITTENINC_PLAYER_H
#define ADVENTURESAREWRITTENINC_PLAYER_H

#include <ncurses.h>

#define RACE_HUMAN 1
#define RACE_DWARF 2
#define RACE_ELF 3
#define RACE_ORC 4

#define CLASS_WARRIOR 1
#define CLASS_WIZARD 2
#define CLASS_ROGUE 3

#define MAX_NAME_LENGTH 16

typedef struct Map_ Map;

typedef struct Player_ {
    WINDOW *win;
    char name[MAX_NAME_LENGTH + 1];
    int x;
    int y;
    int race;
    int class;
    int level;
    int exp;
    int str;
    int end;
    int wis;
    int psy;
    int agi;
    int lck;
} Player;

void init_character(Player *player);

const char *race_str(int race);

const char *class_str(int class);

void draw_player(const Player *player, const Map *map);

#endif //ADVENTURESAREWRITTENINC_PLAYER_H
