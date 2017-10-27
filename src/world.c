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
#include "world.h"
#include "adventures.h"
#include "utils.h"

const long YEAR = MINUTES_PER_HOUR * HOURS_PER_DAY * DAYS_PER_MONTH * MONTHS_PER_YEAR;
const long MONTH = MINUTES_PER_HOUR * HOURS_PER_DAY * DAYS_PER_MONTH;
const long DAY = MINUTES_PER_HOUR * HOURS_PER_DAY;
const long HOUR = MINUTES_PER_HOUR;

int validate_size_input(int input) {
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

void input_world_params(WorldParams *worldParams) {
    refresh();
    printdl(stdscr, 0, 5, 16, "Let's create the world!");
    WINDOW *setup = newwin(24, 64, 7, 0);
    unsigned int w[] = {512, 1024, 2048, 4096, 8192};
    unsigned int h[] = {512, 1024, 2048, 4096, 8192};

    mvwprintw(setup, 0, 0, "Select World Size:");
    for (int i = 0; i < 4; i++) {
        mvwprintw(setup, i + 1, 0, "[%d] %dx%d", i + 1, w[i], h[i]);
        wrefresh(setup);
    }

    int sel;
    do {
        sel = validate_size_input(getch());
    } while (sel == -1);
    worldParams->width = w[sel];
    worldParams->height = h[sel];

    

    wclear(setup);
    wrefresh(setup);
    delwin(setup);
    move(5, 0);
    clrtoeol();
    refresh();
}

long get_year(long time) {
    return BASE_YEAR + time / YEAR;
}

long remove_year(long time) {
    return time - (time / YEAR) * YEAR;
}

long get_month(long time) {
    time = remove_year(time);
    return time / MONTH;
}

long remove_month(long time) {
    return remove_year(time) - get_month(time) * MONTH;
}

long get_day(long time) {
    time = remove_month(time);
    return time / DAY;
}

long remove_day(long time) {
    return remove_month(time) - get_day(time) * DAY;
}

long get_hour(long time) {
    time = remove_day(time);
    return time / HOUR;
}

long get_minute(long time) {
    return time % MINUTES_PER_HOUR;
}

int is_night(long time) {
    long h = get_hour(time);
    if (h < 6 || h > 18) {
        return TRUE;
    }
    return FALSE;
}

void init_world(World *world) {
    world->time = 0;
}

void draw_time(WINDOW *win, int x, int y, long time) {
    mvwprintw(win, y, x, "%02d.%02d.%d", 1 + get_day(time), 1 + get_month(time), get_year(time));
    mvwprintw(win, y + 1, x, "%02d:%02d", get_hour(time), get_minute(time));
}

void draw_world(World *world) {
    werase(world->win);
    mvwprintw(world->win, 0, 0, "Adventures are written in C");
    int startX = 0;
    int startY = 1;
    draw_time(world->win, startX, startY, world->time);
    wrefresh(world->win);
}
