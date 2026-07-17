/*
 * ARM-2D generic loader for RP2040 Q16 FLIP particles.
 * It mirrors the fire loader style: cache a small simulation buffer, then scale it
 * to the target region. 16-bit RGB565 only.
 */
#define __GENERIC_LOADER_INHERIT__
#define __flip_sim_IMPLEMENT__

#include "user_generic_loader_flip.h"
#include "flip_rp2040_q16.h"

#if defined(RTE_Acceleration_Arm_2D_Helper_PFB) && defined(RTE_Acceleration_Arm_2D_Extra_Loader)

#include <assert.h>
#include <string.h>

#undef this
#define this    (*ptThis)

#ifndef FLIP_VIEW_W
#   define FLIP_VIEW_W                 60
#endif
#ifndef FLIP_VIEW_H
#   define FLIP_VIEW_H                 60
#endif
#ifndef FLIP_FAST_SCALE
#   define FLIP_FAST_SCALE             4
#endif
#define FLIP_PALETTE_SIZE              32u
#define FLIP_DENSITY_LEVELS            (FLIP_PALETTE_SIZE - 2u)
#define FLIP_DENSITY_SCALE_SHIFT       12u

ARM_NONNULL(1)
static arm_2d_err_t __flip_sim_decoder_init(arm_generic_loader_t *ptObj);

ARM_NONNULL(1, 2, 3)
static arm_2d_err_t __flip_sim_draw(arm_generic_loader_t *ptObj,
                                    arm_2d_region_t *ptROI,
                                    uint8_t *pchBuffer,
                                    uint32_t iTargetStrideInByte,
                                    uint_fast8_t chBitsPerPixel);

typedef struct flip_resample_map_t {
    uint16_t hwIndex;
    uint8_t chFraction;
} flip_resample_map_t;

static const uint16_t c_hwFlipPalette[FLIP_PALETTE_SIZE] = {
    0x0000u, 0x971Fu, 0x8EDFu, 0x86BFu, 0x7E7Fu, 0x765Fu, 0x6E1Fu, 0x65FFu,
    0x5DBFu, 0x4D9Fu, 0x455Fu, 0x453Fu, 0x3CFFu, 0x3CDEu, 0x34BEu, 0x347Eu,
    0x345Eu, 0x2C3Eu, 0x2BFEu, 0x23DDu, 0x23BDu, 0x1B7Du, 0x1B5Du, 0x1B1Cu,
    0x12FBu, 0x12BBu, 0x127Au, 0x1259u, 0x1218u, 0x09D8u, 0x09B7u, 0x0976u,
};

static uint8_t s_chFlipPaletteIndex[FLIP_VIEW_W * FLIP_VIEW_H];
static uint16_t s_hwFlipPreviousDensityRow[FLIP_VIEW_W];
static flip_resample_map_t s_tFlipMapX[FLIP_VIEW_W];
static flip_resample_map_t s_tFlipMapY[FLIP_VIEW_H];
static int16_t s_nFlipMappedGridW;
static int16_t s_nFlipMappedGridH;
static uint32_t s_wCachedFrameNr;
static flip_fluid_t *s_ptCachedFluid = NULL;
static bool s_bCacheValid = false;

static void __flip_build_resample_map(flip_resample_map_t *ptMap,
                                      int16_t nOutputSize,
                                      int16_t nSourceSize)
{
    const uint32_t wSourceSpan = (uint32_t)(nSourceSize - 1);
    const uint32_t wOutputSpan = (uint32_t)(nOutputSize - 1);

    for (int16_t i = 0; i < nOutputSize; i++) {
        const uint32_t wPositionQ8 = ((uint32_t)i * wSourceSpan << 8)
                                   / wOutputSpan;
        ptMap[i].hwIndex = (uint16_t)(wPositionQ8 >> 8);
        ptMap[i].chFraction = (uint8_t)wPositionQ8;
    }
}

static void __flip_prepare_resample_maps(const flip_fluid_t *f)
{
    const int16_t nGridW = f->numX - 2;
    const int16_t nGridH = f->numY - 2;

    if ((s_nFlipMappedGridW == nGridW) && (s_nFlipMappedGridH == nGridH)) {
        return;
    }

    __flip_build_resample_map(s_tFlipMapX, FLIP_VIEW_W, nGridW);
    __flip_build_resample_map(s_tFlipMapY, FLIP_VIEW_H, nGridH);
    s_nFlipMappedGridW = nGridW;
    s_nFlipMappedGridH = nGridH;
}

static inline uint16_t __flip_lerp_u16(uint16_t a, uint16_t b, uint8_t fraction)
{
    return (uint16_t)((((uint32_t)a * (256u - fraction))
                     + ((uint32_t)b * fraction)
                     + 128u) >> 8);
}

static inline uint16_t __flip_density_at(const flip_fluid_t *f,
                                         uint16_t x,
                                         uint16_t y)
{
    const int16_t gridX = (int16_t)x + 1;
    const int16_t gridY = (f->numY - 2) - (int16_t)y;
    return f->particleDensity[gridX * f->numY + gridY];
}

static uint16_t __flip_resample_density(const flip_fluid_t *f,
                                        const flip_resample_map_t *ptMapX,
                                        const flip_resample_map_t *ptMapY)
{
    const uint16_t x0 = ptMapX->hwIndex;
    const uint16_t y0 = ptMapY->hwIndex;
    const uint16_t xLimit = (uint16_t)(f->numX - 3);
    const uint16_t yLimit = (uint16_t)(f->numY - 3);
    const uint16_t x1 = (x0 < xLimit) ? x0 + 1u : xLimit;
    const uint16_t y1 = (y0 < yLimit) ? y0 + 1u : yLimit;
    const uint16_t d0 = __flip_lerp_u16(__flip_density_at(f, x0, y0),
                                       __flip_density_at(f, x1, y0),
                                       ptMapX->chFraction);
    const uint16_t d1 = __flip_lerp_u16(__flip_density_at(f, x0, y1),
                                       __flip_density_at(f, x1, y1),
                                       ptMapX->chFraction);

    return __flip_lerp_u16(d0, d1, ptMapY->chFraction);
}

static inline uint32_t __flip_abs_q16(q16_t value)
{
    const uint32_t bits = (uint32_t)value;
    return (value < 0) ? (~bits + 1u) : bits;
}

static uint8_t __flip_velocity_highlight(const flip_fluid_t *f,
                                         const flip_resample_map_t *ptMapX,
                                         const flip_resample_map_t *ptMapY)
{
    const int16_t gridX = (int16_t)ptMapX->hwIndex + 1;
    const int16_t gridY = (f->numY - 2) - (int16_t)ptMapY->hwIndex;
    const int16_t cell = gridX * f->numY + gridY;
    const uint32_t au = __flip_abs_q16(f->u[cell]);
    const uint32_t av = __flip_abs_q16(f->v[cell]);
    const uint32_t speed = (au > av) ? au + (av >> 1) : av + (au >> 1);
    const uint32_t highlight = speed >> 17;

    return (uint8_t)((highlight > 3u) ? 3u : highlight);
}

static uint8_t __flip_density_palette_index(uint16_t density,
                                            uint32_t densityScaleQ12)
{
    if (0u == density) {
        return 0u;
    }

    uint32_t level = ((uint32_t)density * densityScaleQ12
                    + (1u << (FLIP_DENSITY_SCALE_SHIFT - 1u)))
                   >> FLIP_DENSITY_SCALE_SHIFT;
    if (level > FLIP_DENSITY_LEVELS) {
        level = FLIP_DENSITY_LEVELS;
    }
    return (uint8_t)(level + 1u);
}

static inline uint32_t __flip_mul_uq16(uint32_t a, uint32_t b)
{
#if defined(__ARM_ARCH_6M__)
    const uint32_t aLo = a & UINT16_MAX;
    const uint32_t bLo = b & UINT16_MAX;
    const uint32_t aHi = a >> 16;
    const uint32_t bHi = b >> 16;

    return ((aLo * bLo) >> 16)
         + (aLo * bHi)
         + (aHi * bLo)
         + ((aHi * bHi) << 16);
#else
    return (uint32_t)(((uint64_t)a * b) >> 16);
#endif
}

static inline int16_t __flip_q16_to_view_x(q16_t x, const flip_fluid_t *f)
{
    q16_t relative = x - f->h;
    uint32_t cellQ16 = 0;
    if (relative > 0) {
        cellQ16 = __flip_mul_uq16((uint32_t)relative, (uint32_t)f->hInv);
    }

    int32_t vx;
    if (FLIP_VIEW_W == (f->numX - 2)) {
        vx = (int32_t)(cellQ16 >> 16);
    } else {
        vx = (int32_t)(((uint64_t)cellQ16 * FLIP_VIEW_W)
                     / ((uint32_t)(f->numX - 2) << 16));
    }
    if (vx < 0) vx = 0;
    if (vx >= FLIP_VIEW_W) vx = FLIP_VIEW_W - 1;
    return (int16_t)vx;
}

static inline int16_t __flip_q16_to_view_y(q16_t y, const flip_fluid_t *f)
{
    q16_t relative = y - f->h;
    uint32_t cellQ16 = 0;
    if (relative > 0) {
        cellQ16 = __flip_mul_uq16((uint32_t)relative, (uint32_t)f->hInv);
    }

    int32_t cellY;
    if (FLIP_VIEW_H == (f->numY - 2)) {
        cellY = (int32_t)(cellQ16 >> 16);
    } else {
        cellY = (int32_t)(((uint64_t)cellQ16 * FLIP_VIEW_H)
                        / ((uint32_t)(f->numY - 2) << 16));
    }
    int32_t vy = FLIP_VIEW_H - 1 - cellY;
    if (vy < 0) vy = 0;
    if (vy >= FLIP_VIEW_H) vy = FLIP_VIEW_H - 1;
    return (int16_t)vy;
}

static void __flip_plot_particle(int16_t x, int16_t y, uint8_t index)
{
    if ((uint16_t)x >= FLIP_VIEW_W || (uint16_t)y >= FLIP_VIEW_H) {
        return;
    }
    s_chFlipPaletteIndex[y * FLIP_VIEW_W + x] = index;
    if ((uint16_t)(x + 1) < FLIP_VIEW_W) s_chFlipPaletteIndex[y * FLIP_VIEW_W + x + 1] = index;
    if ((uint16_t)(y + 1) < FLIP_VIEW_H) s_chFlipPaletteIndex[(y + 1) * FLIP_VIEW_W + x] = index;
    if ((uint16_t)(x + 1) < FLIP_VIEW_W && (uint16_t)(y + 1) < FLIP_VIEW_H) {
        s_chFlipPaletteIndex[(y + 1) * FLIP_VIEW_W + x + 1] = index;
    }
}

static void __flip_sim_update_palette_cache(flip_fluid_t *f)
{
    if (NULL == f) {
        return;
    }
    if (s_bCacheValid && (s_ptCachedFluid == f) && (s_wCachedFrameNr == flip_scene.frameNr)) {
        return;
    }

    memset(s_chFlipPaletteIndex, 0, sizeof(s_chFlipPaletteIndex));

    if (flip_scene.showGrid) {
        const uint32_t denominator = (uint32_t)f->particleRestDensity * 2u;
        const uint32_t densityScaleQ12 = (denominator > 0u)
                                       ? (((uint32_t)FLIP_DENSITY_LEVELS
                                           << FLIP_DENSITY_SCALE_SHIFT)
                                          + (denominator >> 1)) / denominator
                                       : 0u;

        __flip_prepare_resample_maps(f);
        memset(s_hwFlipPreviousDensityRow, 0, sizeof(s_hwFlipPreviousDensityRow));

        for (int16_t y = 0; y < FLIP_VIEW_H; y++) {
            for (int16_t x = 0; x < FLIP_VIEW_W; x++) {
                const uint16_t density = __flip_resample_density(
                    f,
                    &s_tFlipMapX[x],
                    &s_tFlipMapY[y]);
                uint8_t index = __flip_density_palette_index(
                    density,
                    densityScaleQ12);

                if ((index > 1u) && (f->particleRestDensity > 0u)
                 && (density > (f->particleRestDensity >> 3))
                 && ((uint32_t)s_hwFlipPreviousDensityRow[x] * 3u < density)) {
                    const uint8_t highlight = (index > 4u) ? 4u : (uint8_t)(index - 1u);
                    index -= highlight;
                }

                if (index > 1u) {
                    const uint8_t highlight = __flip_velocity_highlight(
                        f,
                        &s_tFlipMapX[x],
                        &s_tFlipMapY[y]);
                    index = (index > (uint8_t)(highlight + 1u))
                          ? (uint8_t)(index - highlight)
                          : 1u;
                }

                s_chFlipPaletteIndex[y * FLIP_VIEW_W + x] = index;
                s_hwFlipPreviousDensityRow[x] = density;
            }
        }
    } else {
        for (int16_t i = 0; i < f->numParticles; i++) {
            int16_t x = __flip_q16_to_view_x(f->particlePos[2 * i], f);
            int16_t y = __flip_q16_to_view_y(f->particlePos[2 * i + 1], f);
            const uint16_t colour = flip_get_particle_colour(f, i);
            uint8_t index = 16u;
            if (colour == flip_density_to_rgb565(0)) index = 2u;
            else if (colour == flip_density_to_rgb565((q16_t)45875)) index = 9u;
            else if (colour == flip_density_to_rgb565(FLIP_Q16_ONE)) index = 19u;
            else if (colour == flip_density_to_rgb565((q16_t)98304)) index = 28u;
            __flip_plot_particle(x, y, index);
        }
    }

    s_ptCachedFluid = f;
    s_wCachedFrameNr = flip_scene.frameNr;
    s_bCacheValid = true;
}

static inline void __flip_store_rgb565(uint8_t *pchTarget, uint16_t hwColour)
{
    memcpy(pchTarget, &hwColour, sizeof(hwColour));
}

static void __flip_sim_draw_fast_scaled(arm_2d_region_t *ptROI,
                                        uint8_t *pchBuffer,
                                        uint32_t iTargetStrideInByte)
{
    const int_fast16_t xStart = ptROI->tLocation.iX;
    const int_fast16_t xLimit = xStart + ptROI->tSize.iWidth;
    const int_fast16_t yStart = ptROI->tLocation.iY;
    const int_fast16_t yLimit = yStart + ptROI->tSize.iHeight;
    const uint32_t nCopyBytes = (uint32_t)ptROI->tSize.iWidth * sizeof(uint16_t);
    int_fast16_t sourceY = yStart / FLIP_FAST_SCALE;
    int_fast16_t rowPhase = yStart - sourceY * FLIP_FAST_SCALE;

    for (int_fast16_t y = yStart; y < yLimit; y++) {
        if ((y > yStart) && (0 != rowPhase)) {
            memcpy(pchBuffer, pchBuffer - iTargetStrideInByte, nCopyBytes);
        } else {
            const uint8_t *pchSource = &s_chFlipPaletteIndex[sourceY * FLIP_VIEW_W];
            uint8_t *pchTarget = pchBuffer;
            int_fast16_t x = xStart;
            int_fast16_t sourceX = x / FLIP_FAST_SCALE;
            int_fast16_t nRun = FLIP_FAST_SCALE
                              - (x - sourceX * FLIP_FAST_SCALE);

            while (x < xLimit) {
                if ((x + nRun) > xLimit) nRun = xLimit - x;
                const uint16_t hwColour = c_hwFlipPalette[pchSource[sourceX++]];
                for (int_fast16_t i = 0; i < nRun; i++) {
                    __flip_store_rgb565(pchTarget, hwColour);
                    pchTarget += sizeof(uint16_t);
                }
                x += nRun;
                nRun = FLIP_FAST_SCALE;
            }
        }
        pchBuffer += iTargetStrideInByte;
        rowPhase++;
        if (rowPhase >= FLIP_FAST_SCALE) {
            rowPhase = 0;
            sourceY++;
        }
    }
}

ARM_NONNULL(1,2)
void flip_sim_show(flip_sim_t *ptThis,
                   const arm_2d_tile_t *ptTile,
                   const arm_2d_region_t *ptRegion,
                   bool bIsNewFrame)
{
    ARM_2D_UNUSED(bIsNewFrame);
    assert(NULL != ptThis);
    if (-1 == (intptr_t)ptTile) {
        ptTile = arm_2d_get_default_frame_buffer();
    }
    arm_2d_tile_copy_only(&this.tTile, ptTile, ptRegion);
}

arm_2d_err_t flip_sim_init(flip_sim_t *ptThis, flip_sim_cfg_t *ptCFG)
{
    assert(NULL != ptThis);
    assert(NULL != ptCFG);
    memset(ptThis, 0, sizeof(flip_sim_t));
    s_ptCachedFluid = NULL;
    s_wCachedFrameNr = 0;
    s_bCacheValid = false;
    this.tCFG = *ptCFG;

    arm_generic_loader_cfg_t tCFG = {
        .bUseHeapForVRES = this.tCFG.bUseHeapForVRES,
        .tColourInfo.chScheme = ARM_2D_COLOUR,
        .bBlendWithBG = false,
        .ImageIO = {
            .ptIO = this.tCFG.ImageIO.ptIO,
            .pTarget = this.tCFG.ImageIO.pTarget,
        },
        .UserDecoder = {
            .fnDecoderInit = &__flip_sim_decoder_init,
            .fnDecode = &__flip_sim_draw,
        },
        .ptScene = this.tCFG.ptScene,
    };

    arm_2d_err_t tResult = arm_generic_loader_init(&this.use_as__arm_generic_loader_t, &tCFG);
    if (tResult < 0) {
        return tResult;
    }

    this.tTile.tRegion.tSize = this.tCFG.tSize;
    if ((0 == this.tTile.tRegion.tSize.iWidth) || (0 == this.tTile.tRegion.tSize.iHeight)) {
        return ARM_2D_ERR_INVALID_PARAM;
    }
    return ARM_2D_ERR_NONE;
}

ARM_NONNULL(1)
void flip_sim_depose(flip_sim_t *ptThis)
{
    assert(NULL != ptThis);
    arm_generic_loader_depose(&this.use_as__arm_generic_loader_t);
}

ARM_NONNULL(1)
void flip_sim_on_load(flip_sim_t *ptThis)
{
    assert(NULL != ptThis);
    arm_generic_loader_on_load(&this.use_as__arm_generic_loader_t);
}

ARM_NONNULL(1)
void flip_sim_on_frame_start(flip_sim_t *ptThis)
{
    assert(NULL != ptThis);
    arm_generic_loader_on_frame_start(&this.use_as__arm_generic_loader_t);
}

ARM_NONNULL(1)
void flip_sim_on_frame_complete(flip_sim_t *ptThis)
{
    assert(NULL != ptThis);
    arm_generic_loader_on_frame_complete(&this.use_as__arm_generic_loader_t);
}

static arm_2d_err_t __flip_sim_decoder_init(arm_generic_loader_t *ptObj)
{
    assert(NULL != ptObj);
    ARM_2D_UNUSED(ptObj);
    return ARM_2D_ERR_NONE;
}

static arm_2d_err_t __flip_sim_draw(arm_generic_loader_t *ptObj,
                                    arm_2d_region_t *ptROI,
                                    uint8_t *pchBuffer,
                                    uint32_t iTargetStrideInByte,
                                    uint_fast8_t chBitsPerPixel)
{
    assert(NULL != ptObj);
    flip_sim_t *ptThis = (flip_sim_t *)ptObj;

#define SCREEN_W this.tCFG.tSize.iWidth
#define SCREEN_H this.tCFG.tSize.iHeight

    if (16 != chBitsPerPixel) {
        return ARM_2D_ERR_NONE;
    }

    flip_fluid_t *f = flip_scene.fluid;
    if (NULL == f) {
        const uint32_t nLineBytes = (uint32_t)ptROI->tSize.iWidth * sizeof(uint16_t);
        for (int_fast16_t y = 0; y < ptROI->tSize.iHeight; y++) {
            memset(pchBuffer, 0, nLineBytes);
            pchBuffer += iTargetStrideInByte;
        }
        return ARM_2D_ERR_NONE;
    }

    __flip_sim_update_palette_cache(f);

    int_fast16_t iXLimit = ptROI->tSize.iWidth + ptROI->tLocation.iX;
    int_fast16_t iYLimit = ptROI->tSize.iHeight + ptROI->tLocation.iY;

    if ((SCREEN_W == (FLIP_VIEW_W * FLIP_FAST_SCALE))
     && (SCREEN_H == (FLIP_VIEW_H * FLIP_FAST_SCALE))
     && (ptROI->tLocation.iX >= 0)
     && (ptROI->tLocation.iY >= 0)
     && (iXLimit <= SCREEN_W)
     && (iYLimit <= SCREEN_H)) {
        __flip_sim_draw_fast_scaled(ptROI, pchBuffer, iTargetStrideInByte);
        return ARM_2D_ERR_NONE;
    }

    const int32_t fxStep = ((int32_t)FLIP_VIEW_W << 16) / SCREEN_W;
    const int32_t fyStep = ((int32_t)FLIP_VIEW_H << 16) / SCREEN_H;
    int32_t fyAcc = (int32_t)ptROI->tLocation.iY * fyStep;

    for (int_fast16_t iY = ptROI->tLocation.iY; iY < iYLimit; iY++) {
        uint8_t *pchPixelLine = pchBuffer;
        int fy = (fyAcc <= 0) ? 0 : (fyAcc / 65536);
        if (fy < 0) fy = 0;
        if (fy >= FLIP_VIEW_H) fy = FLIP_VIEW_H - 1;
        int32_t fxAcc = (int32_t)ptROI->tLocation.iX * fxStep;

        for (int_fast16_t iX = ptROI->tLocation.iX; iX < iXLimit; iX++) {
            int fx = (fxAcc <= 0) ? 0 : (fxAcc / 65536);
            if (fx < 0) fx = 0;
            if (fx >= FLIP_VIEW_W) fx = FLIP_VIEW_W - 1;
            __flip_store_rgb565(
                pchPixelLine,
                c_hwFlipPalette[s_chFlipPaletteIndex[fy * FLIP_VIEW_W + fx]]);
            pchPixelLine += sizeof(uint16_t);
            fxAcc += fxStep;
        }
        pchBuffer += iTargetStrideInByte;
        fyAcc += fyStep;
    }

    return ARM_2D_ERR_NONE;
}

#endif

