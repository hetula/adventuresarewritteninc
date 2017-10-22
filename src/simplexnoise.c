/*
 * OpenSimplex (Simplectic) Noise in C.
 * Ported by Stephen M. Cameron from Kurt Spencer's java implementation
 * Removed 3D and 4D noise generation as not needed
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "simplexnoise.h"

#define STRETCH_CONSTANT_2D (-0.211324865405187)    /* (1 / sqrt(2 + 1) - 1 ) / 2; */
#define SQUISH_CONSTANT_2D  (0.366025403784439)     /* (sqrt(2 + 1) -1) / 2; */

#define NORM_CONSTANT_2D (47.0)

/*
 * Gradients for 2D. They approximate the directions to the
 * vertices of an octagon from the center.
 */
static const int8_t gradients2D[] = {
        5, 2, 2, 5,
        -5, 2, -2, 5,
        5, -2, 2, -5,
        -5, -2, -2, -5,
};

static double extrapolate2(struct SimplexNoiseContext *ctx, int xsb, int ysb, double dx, double dy) {
    int16_t *perm = ctx->perm;
    int index = perm[(perm[xsb & 0xFF] + ysb) & 0xFF] & 0x0E;
    return gradients2D[index] * dx
           + gradients2D[index + 1] * dy;
}

static inline int fastFloor(double x) {
    int xi = (int) x;
    return x < xi ? xi - 1 : xi;
}

static int allocate_perm(struct SimplexNoiseContext *ctx, int nperm) {
    if (ctx->perm)
        free(ctx->perm);
    ctx->perm = (int16_t *) malloc(sizeof(*ctx->perm) * nperm);
    if (!ctx->perm)
        return -ENOMEM;
    return 0;
}

/*
 * Initializes using a permutation array generated from a 64-bit seed.
 * Generates a proper permutation (i.e. doesn't merely perform N successive pair
 * swaps on a base array).  Uses a simple 64-bit LCG.
 */
int simplex_noise_init(int64_t seed, struct SimplexNoiseContext **ctx) {
    int rc;
    int16_t source[256];
    int i;
    int16_t *perm;
    int r;

    *ctx = (struct SimplexNoiseContext *) malloc(sizeof(**ctx));
    if (!(*ctx)) {
        return -ENOMEM;
    }
    (*ctx)->perm = NULL;

    rc = allocate_perm(*ctx, 256);
    if (rc) {
        free(*ctx);
        return rc;
    }

    perm = (*ctx)->perm;

    for (i = 0; i < 256; i++) {
        source[i] = (int16_t) i;
    }
    seed = seed * 6364136223846793005LL + 1442695040888963407LL;
    seed = seed * 6364136223846793005LL + 1442695040888963407LL;
    seed = seed * 6364136223846793005LL + 1442695040888963407LL;
    for (i = 255; i >= 0; i--) {
        seed = seed * 6364136223846793005LL + 1442695040888963407LL;
        r = (int) ((seed + 31) % (i + 1));
        if (r < 0)
            r += (i + 1);
        perm[i] = source[r];
        source[r] = source[i];
    }
    return 0;
}

void simplex_noise_free(struct SimplexNoiseContext *ctx) {
    if (!ctx) {
        return;
    }
    if (ctx->perm) {
        free(ctx->perm);
        ctx->perm = NULL;
    }
    free(ctx);
}

/* 2D OpenSimplex (Simplectic) Noise. */
double simplex_noise(struct SimplexNoiseContext *ctx, double x, double y) {

    /* Place input coordinates onto grid. */
    double stretchOffset = (x + y) * STRETCH_CONSTANT_2D;
    double xs = x + stretchOffset;
    double ys = y + stretchOffset;

    /* Floor to get grid coordinates of rhombus (stretched square) super-cell origin. */
    int xsb = fastFloor(xs);
    int ysb = fastFloor(ys);

    /* Skew out to get actual coordinates of rhombus origin. We'll need these later. */
    double squishOffset = (xsb + ysb) * SQUISH_CONSTANT_2D;
    double xb = xsb + squishOffset;
    double yb = ysb + squishOffset;

    /* Compute grid coordinates relative to rhombus origin. */
    double xins = xs - xsb;
    double yins = ys - ysb;

    /* Sum those together to get a value that determines which region we're in. */
    double inSum = xins + yins;

    /* Positions relative to origin point. */
    double dx0 = x - xb;
    double dy0 = y - yb;

    /* We'll be defining these inside the next block and using them afterwards. */
    double dx_ext, dy_ext;
    int xsv_ext, ysv_ext;

    double dx1;
    double dy1;
    double attn1;
    double dx2;
    double dy2;
    double attn2;
    double zins;
    double attn0;
    double attn_ext;

    double value = 0;

    /* Contribution (1,0) */
    dx1 = dx0 - 1 - SQUISH_CONSTANT_2D;
    dy1 = dy0 - 0 - SQUISH_CONSTANT_2D;
    attn1 = 2 - dx1 * dx1 - dy1 * dy1;
    if (attn1 > 0) {
        attn1 *= attn1;
        value += attn1 * attn1 * extrapolate2(ctx, xsb + 1, ysb + 0, dx1, dy1);
    }

    /* Contribution (0,1) */
    dx2 = dx0 - 0 - SQUISH_CONSTANT_2D;
    dy2 = dy0 - 1 - SQUISH_CONSTANT_2D;
    attn2 = 2 - dx2 * dx2 - dy2 * dy2;
    if (attn2 > 0) {
        attn2 *= attn2;
        value += attn2 * attn2 * extrapolate2(ctx, xsb + 0, ysb + 1, dx2, dy2);
    }

    if (inSum <= 1) { /* We're inside the triangle (2-Simplex) at (0,0) */
        zins = 1 - inSum;
        if (zins > xins || zins > yins) { /* (0,0) is one of the closest two triangular vertices */
            if (xins > yins) {
                xsv_ext = xsb + 1;
                ysv_ext = ysb - 1;
                dx_ext = dx0 - 1;
                dy_ext = dy0 + 1;
            } else {
                xsv_ext = xsb - 1;
                ysv_ext = ysb + 1;
                dx_ext = dx0 + 1;
                dy_ext = dy0 - 1;
            }
        } else { /* (1,0) and (0,1) are the closest two vertices. */
            xsv_ext = xsb + 1;
            ysv_ext = ysb + 1;
            dx_ext = dx0 - 1 - 2 * SQUISH_CONSTANT_2D;
            dy_ext = dy0 - 1 - 2 * SQUISH_CONSTANT_2D;
        }
    } else { /* We're inside the triangle (2-Simplex) at (1,1) */
        zins = 2 - inSum;
        if (zins < xins || zins < yins) { /* (0,0) is one of the closest two triangular vertices */
            if (xins > yins) {
                xsv_ext = xsb + 2;
                ysv_ext = ysb + 0;
                dx_ext = dx0 - 2 - 2 * SQUISH_CONSTANT_2D;
                dy_ext = dy0 + 0 - 2 * SQUISH_CONSTANT_2D;
            } else {
                xsv_ext = xsb + 0;
                ysv_ext = ysb + 2;
                dx_ext = dx0 + 0 - 2 * SQUISH_CONSTANT_2D;
                dy_ext = dy0 - 2 - 2 * SQUISH_CONSTANT_2D;
            }
        } else { /* (1,0) and (0,1) are the closest two vertices. */
            dx_ext = dx0;
            dy_ext = dy0;
            xsv_ext = xsb;
            ysv_ext = ysb;
        }
        xsb += 1;
        ysb += 1;
        dx0 = dx0 - 1 - 2 * SQUISH_CONSTANT_2D;
        dy0 = dy0 - 1 - 2 * SQUISH_CONSTANT_2D;
    }

    /* Contribution (0,0) or (1,1) */
    attn0 = 2 - dx0 * dx0 - dy0 * dy0;
    if (attn0 > 0) {
        attn0 *= attn0;
        value += attn0 * attn0 * extrapolate2(ctx, xsb, ysb, dx0, dy0);
    }

    /* Extra Vertex */
    attn_ext = 2 - dx_ext * dx_ext - dy_ext * dy_ext;
    if (attn_ext > 0) {
        attn_ext *= attn_ext;
        value += attn_ext * attn_ext * extrapolate2(ctx, xsv_ext, ysv_ext, dx_ext, dy_ext);
    }

    return (value / NORM_CONSTANT_2D + 1) / 2;
}
