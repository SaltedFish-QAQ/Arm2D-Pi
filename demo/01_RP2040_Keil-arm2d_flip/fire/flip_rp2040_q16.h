#ifndef __FLIP_RP2040_Q16_H__
#define __FLIP_RP2040_Q16_H__

#include <stdint.h>
#include "__arm_2d_math.h"
#include "flip_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLIP_Q16_ONE                   ((q16_t)0x00010000)
#define FLIP_Q16_HALF                  ((q16_t)0x00008000)
#define FLIP_Q16_ZERO                  ((q16_t)0)

#define FLIP_FLUID_CELL                0u
#define FLIP_AIR_CELL                  1u
#define FLIP_SOLID_CELL                2u

#define FLIP_HOURGLASS_PROFILE_SIZE    128u

extern const uint8_t
c_chFlipHourglassProfile[FLIP_HOURGLASS_PROFILE_SIZE];

static inline uint8_t flip_hourglass_profile_sample(uint32_t wPositionQ16)
{
    uint32_t wLastIndex = FLIP_HOURGLASS_PROFILE_SIZE - 1u;
    uint32_t wIndex = wPositionQ16 >> 16;

    if (wIndex >= wLastIndex) {
        return c_chFlipHourglassProfile[wLastIndex];
    }

    uint32_t wFraction = (wPositionQ16 >> 8) & 0xFFu;
    uint32_t wValue0 = c_chFlipHourglassProfile[wIndex];
    uint32_t wValue1 = c_chFlipHourglassProfile[wIndex + 1u];
    return (uint8_t)(((wValue0 * (256u - wFraction))
                    + (wValue1 * wFraction)
                    + 128u) >> 8);
}

typedef struct flip_fluid_t {
    int16_t numX;
    int16_t numY;
    int16_t numCells;
    q16_t h;
    q16_t hInv;
    q16_t hHalf;
    q16_t maxX;
    q16_t maxY;
    q16_t density;

    q16_t *u;
    q16_t *v;
    uint16_t *du;
    uint16_t *dv;
    q16_t *prevU;
    q16_t *prevV;
    uint8_t *cellType;
    uint16_t *particleDensity; /* Q4.12 accumulated interpolation weights */
    uint16_t particleRestDensity;

    int16_t maxParticles;
    int16_t numParticles;
    q16_t particleRadius;
    q16_t particleDiameter;
    q16_t pInvSpacing;
    int16_t pNumX;
    int16_t pNumY;
    int16_t pNumCells;

    q16_t *particlePos;       /* x,y interleaved */
    q16_t *particleVel;       /* vx,vy interleaved */
    uint16_t *particleColor;  /* RGB565 cache, optional for direct draw */
    uint8_t *particleColourSideBits;
    uint16_t *cellParticleIds;
    uint16_t *firstCellParticle;
    uint16_t *numCellParticles;
} flip_fluid_t;

typedef struct flip_scene_t {
    q16_t gravity;
    q16_t gravityX;
    q16_t gravityY;
    q16_t dt;
    q16_t flipRatio;
    q16_t overRelaxation;
    int16_t numPressureIters;
    int16_t numParticleIters;
    uint32_t frameNr;
    uint8_t compensateDrift;
    uint8_t separateParticles;
    uint8_t paused;
    uint8_t showGrid;
    flip_fluid_t *fluid;
} flip_scene_t;

extern flip_scene_t flip_scene;

int flip_init(void);
int flip_init_with_grid(int16_t gridW, int16_t gridH, int16_t maxParticles);
void flip_reset(void);
void flip_sim_update(void);
void flip_set_gravity(q16_t gx, q16_t gy);
void flip_set_screen_motion_q15(int16_t gravityX,
                                int16_t gravityY,
                                int16_t accelerationX,
                                int16_t accelerationY);
uint16_t flip_get_particle_colour(const flip_fluid_t *f, int16_t particleIndex);
uint8_t flip_particle_uses_pre_neck_colour(const flip_fluid_t *f,
                                           int16_t particleIndex);
uint16_t flip_density_to_rgb565(q16_t densityRatio);
uint16_t flip_density_sample_to_rgb565(uint16_t density, uint16_t restDensity);

#ifdef __cplusplus
}
#endif

#endif
