#ifndef ADVENTURESAREWRITTENINC_TERRAIN_H
#define ADVENTURESAREWRITTENINC_TERRAIN_H

#define CLR_DAY_LAKE 1
#define CLR_DAY_BEACH 2
#define CLR_DAY_GRASS 3
#define CLR_DAY_FOREST 4
#define CLR_DAY_HILL 5
#define CLR_DAY_MOUNTAIN 6
#define CLR_DAY_SNOWY_MOUNTAIN 7

#define CLR_NIGHT_LAKE 8
#define CLR_NIGHT_BEACH 9
#define CLR_NIGHT_GRASS 10
#define CLR_NIGHT_FOREST 11
#define CLR_NIGHT_HILL 12
#define CLR_NIGHT_MOUNTAIN 13
#define CLR_NIGHT_SNOWY_MOUNTAIN 14

#define TERRAIN_LAKE 0
#define TERRAIN_BEACH 1
#define TERRAIN_GRASS 2
#define TERRAIN_FOREST 3
#define TERRAIN_HILL 4
#define TERRAIN_MOUNTAIN 5
#define TERRAIN_SNOWY_MOUNTAIN 6

struct Terrain_ {
    char visual;
    int color_day;
    int color_night;
    float roughness;
};

typedef const struct Terrain_ Terrain;

extern Terrain LAKE;
extern Terrain BEACH;
extern Terrain GRASS;
extern Terrain FOREST;
extern Terrain HILL;
extern Terrain MOUNTAIN;
extern Terrain SNOWY_MOUNTAIN;

void initialize_colors();

Terrain *get_terrain(int terrain);

#endif //ADVENTURESAREWRITTENINC_TERRAIN_H
