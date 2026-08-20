#ifndef _DATA_H_
#define _DATA_H_

#include "arm_2d.h"
#include "ldConfig.h"

#define FONT_SOURCE_HAN_SANS_SC_NORMAL_22 (arm_2d_font_t*)ldBaseGetVresFont(0x00000000)
#define FONT_SIMSUN_9 (arm_2d_font_t*)ldBaseGetVresFont(0x00000B9C)
#define FONT_SOURCE_HAN_SANS_SC_NORMAL_16 (arm_2d_font_t*)ldBaseGetVresFont(0x00000BC0)

#define IMAGE_MANAGEMENT_64_BMP_MASK NULL
#define IMAGE_CLOCK_64_BMP_MASK NULL
#define IMAGE_CALENDAR_64_BMP_MASK NULL
#define IMAGE_SCHEDULE_64_BMP_MASK NULL
#if LD_CFG_COLOR_DEPTH == 16
#define IMAGE_MANAGEMENT_64_BMP (arm_2d_tile_t*)ldBaseGetVresImage(0x00001520)
#endif
#if LD_CFG_COLOR_DEPTH == 16
#define IMAGE_CLOCK_64_BMP (arm_2d_tile_t*)ldBaseGetVresImage(0x00003530)
#endif
#if LD_CFG_COLOR_DEPTH == 16
#define IMAGE_CALENDAR_64_BMP (arm_2d_tile_t*)ldBaseGetVresImage(0x00005540)
#endif
#if LD_CFG_COLOR_DEPTH == 16
#define IMAGE_SCHEDULE_64_BMP (arm_2d_tile_t*)ldBaseGetVresImage(0x00007550)
#endif

#endif // _DATA_H_
