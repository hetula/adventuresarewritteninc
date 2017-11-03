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
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "adventures.h"
#include "intro.h"
#include "charactercreator.h"
#include "utils.h"
#include "tileobjectgenerator.h"

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

void move_plr(World *world, Map *map, Player *player, int x, int y) {
    player->x += x;
    player->y += y;
    if (player->x < 0) {
        player->x = map->width - 1;
    }
    if (player->y < 0) {
        player->y = map->height - 1;
    }
    if (player->x >= map->width) {
        player->x = 0;
    }
    if (player->y >= map->height) {
        player->y = 0;
    }
    Terrain *terrain = terrain_at(map, player->x, player->y);
    world->time += terrain->travel_cross_time + get_rnd(30);
}

void begin_adventure(Map *map, World *world, Player *player) {
    init_world(world);
    clear();
    refresh();

    int ch;
    while (1) {
        sleep_ms(16);
        draw_world(world);
        draw_player(player, map);
        draw_map(map, world, player);
        ch = getch();
        switch (ch) {
            case KEY_UP:
                move_plr(world, map, player, 0, MOVE_UP);
                break;
            case KEY_DOWN:
                move_plr(world, map, player, 0, MOVE_DOWN);
                break;
            case KEY_RIGHT:
                move_plr(world, map, player, MOVE_RIGHT, 0);
                break;
            case KEY_LEFT:
                move_plr(world, map, player, MOVE_LEFT, 0);
                break;
            case 'q':
                return;
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    if (check_compatible() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    init_curses();

    Map map = {MAP_WIDTH, MAP_HEIGHT};
    Player player;
    World world;

    init_character(&player);

    play_intro(argc == 1 ? TRUE : FALSE);
    if (argc == 1) {
        create_player(&player);
    }
    unsigned int seed = (unsigned int) time(NULL);
    mvprintw(5, 1, "Creating world...\n\t-> Seed:   %u\n\t-> Width:  %u\n\t-> Height: %u",
             seed, MAP_WIDTH, MAP_HEIGHT);
    srandom(seed);
    refresh();

    map.data = calloc(MAP_FULL_SIZE, sizeof(MapTile *));

    for (int i = 0; i < MAP_FULL_SIZE; i++) {
        map.data[i] = malloc(sizeof(MapTile));
        map.data[i]->terrain_type = 0;
        map.data[i]->x = 0;
        map.data[i]->y = 0;
        map.data[i]->tile = NULL;
    }
    generate_map(&map, random(), argc != 1 ? TRUE : FALSE);


    map.win = newwin(MAP_WINDOW_HEIGHT, MAP_WINDOW_WIDTH, 0, 0);
    world.win = newwin(WORLD_WINDOW_HEIGHT, WORLD_WINDOW_WIDTH, 0, MAP_WINDOW_WIDTH + 1);
    player.win = newwin(PLAYER_WINDOW_HEIGHT, PLAYER_WINDOW_WIDTH, WORLD_WINDOW_HEIGHT,
                        MAP_WINDOW_WIDTH + 1);

    player.x = get_rnd(map.width);
    player.y = get_rnd(map.height);
    world.time = get_rnd(3600) + get_rnd(3600) + get_rnd(3600) + get_rnd(3600);

    cbreak();
    begin_adventure(&map, &world, &player);
    delwin(map.win);
    delwin(world.win);
    delwin(player.win);
    endwin();

    for (int i = 0; i < MAP_FULL_SIZE; i++) {
        if (map.data[i]->tile != NULL) {
            free_tile_object(map.data[i]->tile);
            map.data[i]->tile = NULL;
        }
        free(map.data[i]);
        map.data[i] = NULL;
    }
    free(map.data);
    return EXIT_SUCCESS;
}
