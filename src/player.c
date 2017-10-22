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
#include <memory.h>
#include "adventures.h"
#include "player.h"

void create_character(Player *player) {
    player->x = 0;
    player->y = 0;
    player->race = RACE_DWARF;
    player->class = CLASS_WIZARD;
    player->level = 1;
    player->exp = 0;
    player->str = 10;
    player->end = 13;
    player->wis = 24;
    player->psy = 19;
    player->agi = 13;
    player->lck = 11;
    player->name[0] = '\0';
    strncat(player->name, "Gimli", MAX_NAME_LENGTH);
}

void draw_player(const Player *player) {
    int startX = MAP_WINDOW_WIDTH + 1;
    int startY = 5;
    mvprintw(startY++, startX, "%s", player->name);
    mvprintw(startY++, startX, "Dwarven Wizard");
    mvprintw(startY++, startX, "X: %d Y: %d     ", player->x, player->y);
    startY++;
    mvprintw(startY++, startX, "Level:      %d", player->level);
    mvprintw(startY++, startX, "Experience: %d/%d", player->exp, 100);
    mvprintw(startY++, startX, "Strength:   %d", player->str);
    mvprintw(startY++, startX, "Endurenace: %d", player->end);
    mvprintw(startY++, startX, "Wisdom:     %d", player->wis);
    mvprintw(startY++, startX, "Psyche:     %d", player->psy);
    mvprintw(startY++, startX, "Agility:    %d", player->agi);
    mvprintw(startY, startX, "Luck:       %d", player->lck);
    refresh();
}
