#ifndef __UI_IMAGES_H__
#define __UI_IMAGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldConfig.h"

// management_64.bmp < 64x64 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_management_64_bmp_GRAY8;
#define IMAGE_MANAGEMENT_64_BMP          (arm_2d_tile_t*)&c_tile_management_64_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_management_64_bmp_RGB565;
#define IMAGE_MANAGEMENT_64_BMP          (arm_2d_tile_t*)&c_tile_management_64_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_management_64_bmp_CCCN888;
#define IMAGE_MANAGEMENT_64_BMP          (arm_2d_tile_t*)&c_tile_management_64_bmp_CCCN888
#endif
#define IMAGE_MANAGEMENT_64_BMP_MASK     NULL

// clock_64.bmp < 64x64 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_clock_64_bmp_GRAY8;
#define IMAGE_CLOCK_64_BMP          (arm_2d_tile_t*)&c_tile_clock_64_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_clock_64_bmp_RGB565;
#define IMAGE_CLOCK_64_BMP          (arm_2d_tile_t*)&c_tile_clock_64_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_clock_64_bmp_CCCN888;
#define IMAGE_CLOCK_64_BMP          (arm_2d_tile_t*)&c_tile_clock_64_bmp_CCCN888
#endif
#define IMAGE_CLOCK_64_BMP_MASK     NULL

// calendar_64.bmp < 64x64 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_calendar_64_bmp_GRAY8;
#define IMAGE_CALENDAR_64_BMP          (arm_2d_tile_t*)&c_tile_calendar_64_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_calendar_64_bmp_RGB565;
#define IMAGE_CALENDAR_64_BMP          (arm_2d_tile_t*)&c_tile_calendar_64_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_calendar_64_bmp_CCCN888;
#define IMAGE_CALENDAR_64_BMP          (arm_2d_tile_t*)&c_tile_calendar_64_bmp_CCCN888
#endif
#define IMAGE_CALENDAR_64_BMP_MASK     NULL

// schedule_64.bmp < 64x64 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_schedule_64_bmp_GRAY8;
#define IMAGE_SCHEDULE_64_BMP          (arm_2d_tile_t*)&c_tile_schedule_64_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_schedule_64_bmp_RGB565;
#define IMAGE_SCHEDULE_64_BMP          (arm_2d_tile_t*)&c_tile_schedule_64_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_schedule_64_bmp_CCCN888;
#define IMAGE_SCHEDULE_64_BMP          (arm_2d_tile_t*)&c_tile_schedule_64_bmp_CCCN888
#endif
#define IMAGE_SCHEDULE_64_BMP_MASK     NULL

#ifdef __cplusplus
}
#endif

#endif
