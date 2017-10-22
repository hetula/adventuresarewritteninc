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
/* Use POSIX 2008 */
#define _XOPEN_SOURCE 700
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE

#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

int check_compatible() {
    if (isatty(1) == 0) {
        printf("Not a tty\n");
        return EXIT_FAILURE;
    }
    initscr();
    if (has_colors() == FALSE) {
        endwin();
        printf("Terminal not supporting colors!");
        return EXIT_FAILURE;
    }
    return 0;
}

void init_curses() {
    noecho();
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
}

void begin_adventure(Map *map) {
    mvprintw(0, 71, "Adventures are written in C");
    int ch;
    generate_map(map, random(), 3);
    Player player;
    player.x = 0;
    player.y = 0;
    while (1) {
        if (player.x < 0) {
            player.x = 0;
        }
        if (player.y < 0) {
            player.y = 0;
        }
        draw_map(map, &player);
        draw_player(&player);
        ch = getch();
        switch (ch) {
            case 'q':
                return;
            case KEY_UP:
                player.y--;
                break;
            case KEY_DOWN:
                player.y++;
                break;
            case KEY_RIGHT:
                player.x++;
                break;
            case KEY_LEFT:
                player.x--;
                break;
            default:
                break;
        }
    }
}

int main() {
    if (check_compatible() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    srandom((unsigned int) time(NULL));
    init_curses();
    int mapWidth = 512;
    int mapHeight = 512;

    size_t size = 512 * 512;
    int *mapData = calloc(size, sizeof(int));
    Map map = {mapWidth, mapHeight, mapData};
    begin_adventure(&map);
    endwin();
    free(mapData);
    return EXIT_SUCCESS;
}
