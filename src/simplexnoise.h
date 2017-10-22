#ifndef ADVENTURESAREWRITTENINC_SIMPLEXNOISE_H
#define ADVENTURESAREWRITTENINC_SIMPLEXNOISE_H

struct SimplexNoiseContext {
    int16_t *perm;
};

int simplex_noise_init(int64_t seed, struct SimplexNoiseContext **ctx);

void simplex_noise_free(struct SimplexNoiseContext *ctx);

double simplex_noise(struct SimplexNoiseContext *ctx, double x, double y);

#endif //ADVENTURESAREWRITTENINC_SIMPLEXNOISE_H
