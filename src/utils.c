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
#include <memory.h>
#include "utils.h"
#include <stdlib.h>
#include <time.h>

void printdl(WINDOW *window, int startX, int startY, unsigned int delayMs, char *string) {
    size_t l = strlen(string);
    int x = startX, y = startY;
    for (int i = 0; i < l; i++) {
        if (string[i] == '\n') {
            y++;
            x = startX;
            continue;
        }
        mvwprintw(window, y, x, "%c", string[i]);
        wrefresh(window);
        x++;
        sleep_ms(delayMs);
    }
}

void sleep_ms(unsigned int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

int get_rnd(int max) {
    int divisor = RAND_MAX / (max + 1);
    long retval;
    do {
        retval = random() / divisor;
    } while (retval >= max);
    return (int) retval; // Max is type int so result can't be any bigger
}

double manhattan_distance_t(MapTile *tile1, MapTile *tile2) {
    return manhattan_distance(tile1->x, tile1->y, tile2->x, tile2->y);
}

double manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}