#ifndef ADVENTURESAREWRITTENINC_SIMPLEXNOISE_H
#define ADVENTURESAREWRITTENINC_SIMPLEXNOISE_H

typedef struct SimplexNoiseContext_ {
    int16_t *perm;
} SimplexNoiseContext;

int simplex_noise_init(int64_t seed, SimplexNoiseContext **ctx);

void simplex_noise_free(SimplexNoiseContext *ctx);

double simplex_noise4(SimplexNoiseContext *ctx, double x, double y, double z, double w);


#endif //ADVENTURESAREWRITTENINC_SIMPLEXNOISE_H
