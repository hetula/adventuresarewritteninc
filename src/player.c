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
#include "player.h"

void draw_player(const Player *player) {
    int startX = 71;
    int startY = 1;
    mvprintw(startY++, startX, "<PlayerName>");
    mvprintw(startY++, startX, "X: %d Y: %d     ", player->x, player->y);
    mvprintw(startY++, startX, "Dwarven Wizard");
    mvprintw(startY++, startX, "Experience: %d/%d", 0, 100);
    mvprintw(startY++, startX, "Strength:   %d", 12);
    mvprintw(startY++, startX, "Endurenace: %d", 15);
    mvprintw(startY++, startX, "Wisdom:     %d", 32);
    mvprintw(startY++, startX, "Psyche:     %d", 28);
    mvprintw(startY++, startX, "Agility:    %d", 10);
    mvprintw(startY, startX, "Luck:       %d", 6);
    refresh();
}
