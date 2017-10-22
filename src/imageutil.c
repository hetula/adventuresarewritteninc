#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "imageutil.h"

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

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

/* A picture. */

typedef struct {
    pixel_t *pixels;
    size_t width;
    size_t height;
} bitmap_t;

/* Given "bitmap", this returns the pixel of bitmap at the point
   ("x", "y"). */

static pixel_t *pixel_at(bitmap_t *bitmap, int x, int y) {
    return bitmap->pixels + bitmap->width * y + x;
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error. */

static int save_png_to_file(bitmap_t *bitmap, const char *path) {
    FILE *fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int x, y;
    png_byte **row_pointers = NULL;
    /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
    int status = -1;
    /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
    */
    int pixel_size = 3;
    int depth = 8;

    fp = fopen(path, "wb");
    if (!fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }

    /* Set up error handling. */

    if (setjmp (png_jmpbuf(png_ptr))) {
        goto png_failure;
    }

    /* Set image attributes. */

    png_set_IHDR(png_ptr,
                 info_ptr,
                 bitmap->width,
                 bitmap->height,
                 depth,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = png_malloc(png_ptr, bitmap->height * sizeof(png_byte *));
    for (y = 0; y < bitmap->height; y++) {
        png_byte *row =
                png_malloc(png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; x++) {
            pixel_t *pixel = pixel_at(bitmap, x, y);
            *row++ = pixel->red;
            *row++ = pixel->green;
            *row++ = pixel->blue;
        }
    }

    /* Write the image data to "fp". */

    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;

    for (y = 0; y < bitmap->height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    png_failure:
    png_create_info_struct_failed:
    png_destroy_write_struct(&png_ptr, &info_ptr);
    png_create_write_struct_failed:
    fclose(fp);
    fopen_failed:
    return status;
}

/* Given "value" and "max", the maximum value which we expect "value"
   to take, this returns an integer between 0 and 255 proportional to
   "value" divided by "max". */

static void pix(pixel_t *pixel, uint8_t red, uint8_t green, uint8_t blue) {
    pixel->red = red;
    pixel->blue = blue;
    pixel->green = green;

}

static void map_terrain(pixel_t *pxl, int terrain) {
    switch (terrain) {
        case TERRAIN_LAKE:
            pix(pxl, 66, 161, 244);
            break;
        case TERRAIN_BEACH:
            pix(pxl, 242, 220, 135);
            break;
        case TERRAIN_GRASS:
            pix(pxl, 135, 242, 148);
            break;
        case TERRAIN_FOREST:
            pix(pxl, 5, 132, 20);
            break;
        case TERRAIN_HILL:
            pix(pxl, 117, 87, 48);
            break;
        case TERRAIN_MOUNTAIN:
            pix(pxl, 183, 182, 181);
            break;
        case TERRAIN_SNOWY_MOUNTAIN:
            pix(pxl, 255, 255, 255);
            break;
        default:
            pix(pxl, 255, 0, 0);
            break;
    }
}

void save_map(const Map *map) {
    bitmap_t png_map;
    png_map.width = (size_t) map->width;
    png_map.height = (size_t) map->height;
    png_map.pixels = calloc(png_map.width * png_map.height, sizeof(size_t));
    if (!png_map.pixels) {
        return;
    }
    int x, y;
    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            pixel_t *pxl = pixel_at(&png_map, x, y);
            map_terrain(pxl, map->data[x + y * map->width]);
        }
    }
    save_png_to_file(&png_map, "map.png");
    free(png_map.pixels);
}
