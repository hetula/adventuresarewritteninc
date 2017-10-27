/*
 * MIT License
 *
 * Copyright (c) $today.year Tuomo Heino
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
#include "utils.h"

void init() {
    echo();
    curs_set(TRUE);
    keypad(stdscr, FALSE);
}

void end() {
    keypad(stdscr, TRUE);
    clear();
}

void input_name(WINDOW *win, char *name) {
    mvwprintw(win, 1, 0, "Enter name: >");
    wattron(win, COLOR_PAIR(2));
    wgetnstr(win, name, MAX_NAME_LENGTH);
    wattroff(win, COLOR_PAIR(2));
}

void ask_name(WINDOW *win, Player *player) {
    size_t l = (size_t) -1;
    do {
        if (l != -1) {
            wclear(win);
            mvwprintw(win, 0, 0, "Invalid name! Please re-enter!");
            wrefresh(win);
        }
        input_name(win, player->name);
        l = strlen(player->name);
    } while (l <= 0 || l > MAX_NAME_LENGTH);
}

int validate_race_input(int input) {
    switch (input) {
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        case '4':
            return 3;
        default:
            return -1;
    }
}

void ask_race(WINDOW *win, Player *player) {
    mvwprintw(win, 0, 0, "Select race!");
    wrefresh(win);
    int races[] = {RACE_HUMAN, RACE_DWARF, RACE_ELF, RACE_ORC};
    size_t l = sizeof(races) / sizeof(int);
    for (int i = 0; i < l; i++) {
        mvwprintw(win, i + 1, 0, "[%d] %s", i + 1, race_str(races[i]));
        wrefresh(win);
    }
    int sel;
    do {
        sel = validate_race_input(getch());
    } while (sel == -1);
    player->race = races[sel];
}

int validate_class_input(int input) {
    switch (input) {
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        default:
            return -1;
    }
}

void ask_class(WINDOW *win, Player *player) {
    mvwprintw(win, 0, 0, "Select class!");
    wrefresh(win);
    int classes[] = {CLASS_WARRIOR, CLASS_WIZARD, CLASS_ROGUE};
    size_t l = sizeof(classes) / sizeof(int);
    for (int i = 0; i < l; i++) {
        mvwprintw(win, i + 1, 0, "[%d] %s", i + 1, class_str(classes[i]));
        wrefresh(win);
    }
    int sel;
    do {
        sel = validate_class_input(getch());
    } while (sel == -1);
    player->class = classes[sel];
}

void create_player(Player *player) {
    printdl(stdscr, 0, 5, 16, "Let's create a character!");
    WINDOW *setup = newwin(24, 64, 6, 0);
    init();
    init_pair(2, 4, COLOR_BLACK);

    ask_name(setup, player);
    wclear(setup);
    curs_set(FALSE);
    noecho();

    ask_race(setup, player);
    wclear(setup);

    ask_class(setup, player);
    wclear(setup);

    wclear(setup);
    delwin(setup);
    end();
}