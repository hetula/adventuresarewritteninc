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
#include "player.h"

void init_character(Player *player) {
    player->x = 0;
    player->y = 0;
    player->race = RACE_HUMAN;
    player->class = CLASS_WARRIOR;
    player->level = 1;
    player->exp = 0;
    player->str = 1;
    player->end = 1;
    player->wis = 1;
    player->psy = 1;
    player->agi = 1;
    player->lck = 1;
    player->name[0] = '\0';
    strncat(player->name, "Player Name", MAX_NAME_LENGTH);
}



const char *race_str(int race) {
    switch (race) {
        case RACE_HUMAN:
            return "Human";
        case RACE_DWARF:
            return "Dwarf";
        case RACE_ELF:
            return "Elf";
        case RACE_ORC:
            return "Orc";
        default:
            return "Unknown Race";
    }
}

const char *class_str(int class) {
    switch (class) {
        case CLASS_WARRIOR:
            return "Warrior";
        case CLASS_WIZARD:
            return "Wizard";
        case CLASS_ROGUE:
            return "Rogue";
        default:
            return "Unknown Class";
    }
}

void draw_player(const Player *player) {
    WINDOW *win = player->win;
    werase(win);
    int startX = 0;
    int startY = 0;
    size_t l = strlen(player->name);
    for (int i = 0; i < l; i++) {
        mvwprintw(win, startY, startX + i, "%c", player->name[i]);
        wrefresh(win);
    }
    startY++;
    mvwprintw(win, startY++, startX, "%s %s", race_str(player->race), class_str(player->class));
    mvwprintw(win, startY++, startX, "X: %d Y: %d     ", player->x, player->y);
    startY++;
    mvwprintw(win, startY++, startX, "Level:      %d", player->level);
    mvwprintw(win, startY++, startX, "Experience: %d/%d", player->exp, 100);
    mvwprintw(win, startY++, startX, "Strength:   %d", player->str);
    mvwprintw(win, startY++, startX, "Endurenace: %d", player->end);
    mvwprintw(win, startY++, startX, "Wisdom:     %d", player->wis);
    mvwprintw(win, startY++, startX, "Psyche:     %d", player->psy);
    mvwprintw(win, startY++, startX, "Agility:    %d", player->agi);
    mvwprintw(win, startY, startX, "Luck:       %d", player->lck);
    wrefresh(win);
}
