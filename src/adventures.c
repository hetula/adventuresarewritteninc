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
#include "adventures.h"
#include "intro.h"
#include "charactercreator.h"

const int MAP_FEATURE_SIZE = 3;

#define MOVE_LEFT (-1)
#define MOVE_RIGHT 1
#define MOVE_UP (-1)
#define MOVE_DOWN 1

int check_compatible() {
    if (isatty(1) == 0) {
        printf("Not a tty\n");
        return EXIT_FAILURE;
    }
    initscr();
    if (has_colors() == FALSE) {
        endwin();
        printf("Terminal not supporting colors!\n");
        return EXIT_FAILURE;
    }
    return 0;
}

void init_curses() {
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
}

static inline int get_rnd(int max) {
    return (int) (random() % max);
}

void move_plr(World *world, Player *player, int x, int y) {
    player->x += x;
    player->y += y;
    if (player->x < 0) {
        player->x = 0;
    }
    if (player->y < 0) {
        player->y = 0;
    }
    world->time += get_rnd(MINUTES_PER_HOUR * 2);
}

void begin_adventure(Map *map, World *world, Player *player) {
    init_world(world);
    generate_map(map, random(), MAP_FEATURE_SIZE);
    sleep(2);
    clear();
    refresh();

    int ch;
    while (1) {
        usleep(16 * 1000);
        refresh();
        draw_world(world);
        draw_player(player);
        draw_map(map, world, player);
        ch = getch();
        switch (ch) {
            case KEY_UP:
                move_plr(world, player, 0, MOVE_UP);
                break;
            case KEY_DOWN:
                move_plr(world, player, 0, MOVE_DOWN);
                break;
            case KEY_RIGHT:
                move_plr(world, player, MOVE_RIGHT, 0);
                break;
            case KEY_LEFT:
                move_plr(world, player, MOVE_LEFT, 0);
                break;
            case 'o':
                world->time++;
                break;
            case 'l':
                world->time--;
                if (world->time < 0) {
                    world->time = 0;
                }
                break;
            case 'q':
                return;
            default:
                break;
        }
    }
}

int main() {
    if (check_compatible() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    init_curses();
    play_intro();
    Player player;
    init_character(&player);
    create_player(&player);
    WorldParams worldParams = {0, 512, 512};
    input_world_params(&worldParams);
    unsigned int seed = worldParams.seed == 0 ? (unsigned int) time(NULL) : worldParams.seed;
    srandom(seed);
    mvprintw(5, 0, " Creating world...\n\t-> Seed:   %d\n\t-> Width:  %d\n\t-> Height: %d",
             seed, worldParams.width, worldParams.height);
    refresh();

    unsigned int mapWidth = worldParams.width;
    unsigned int mapHeight = worldParams.height;

    int *mapData = calloc(mapWidth * mapHeight, sizeof(int));
    Map map = {newwin(MAP_WINDOW_HEIGHT, MAP_WINDOW_WIDTH, 0, 0), mapWidth, mapHeight, mapData};
    World world;
    world.win = newwin(5, 32, 0, MAP_WINDOW_WIDTH + 1);
    player.win = newwin(16, 32, 5, MAP_WINDOW_WIDTH + 1);

    cbreak();
    begin_adventure(&map, &world, &player);
    delwin(map.win);
    delwin(world.win);
    delwin(player.win);
    endwin();
    free(mapData);
    return EXIT_SUCCESS;
}
