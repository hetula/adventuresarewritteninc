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
#include <unistd.h>
#include "intro.h"
#include "utils.h"

void play_intro() {
    init_pair(1, 12, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    char title[] = "    _      _             _                                               _ _   _              _         ___ \n"
            "   /_\\  __| |_ _____ _ _| |_ _  _ _ _ ___ ___  __ _ _ _ ___  __ __ ___ _(_) |_| |_ ___ _ _   (_)_ _    / __|\n"
            "  / _ \\/ _` \\ V / -_) ' \\  _| || | '_/ -_|_-< / _` | '_/ -_) \\ V  V / '_| |  _|  _/ -_) ' \\  | | ' \\  | (__ \n"
            " /_/ \\_\\__,_|\\_/\\___|_||_\\__|\\_,_|_| \\___/__/ \\__,_|_| \\___|  \\_/\\_/|_| |_|\\__|\\__\\___|_||_| |_|_||_|  \\___|\n"
            "";
    printdl(stdscr, 0, 0, 2, title);
    sleep(1);
    attroff(COLOR_PAIR(1));
}