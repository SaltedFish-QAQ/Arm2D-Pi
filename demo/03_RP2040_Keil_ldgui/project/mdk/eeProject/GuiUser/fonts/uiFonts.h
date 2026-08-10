#ifndef __UI_FONTS_H__
#define __UI_FONTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d_helper.h"

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_Source_Han_Sans_SC_Normal_22_A8;

#define FONT_SOURCE_HAN_SANS_SC_NORMAL_22          (arm_2d_font_t*)&ARM_2D_FONT_Source_Han_Sans_SC_Normal_22_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_SimSun_9_A8;

#define FONT_SIMSUN_9          (arm_2d_font_t*)&ARM_2D_FONT_SimSun_9_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_Source_Han_Sans_SC_Normal_16_A8;

#define FONT_SOURCE_HAN_SANS_SC_NORMAL_16          (arm_2d_font_t*)&ARM_2D_FONT_Source_Han_Sans_SC_Normal_16_A8

#ifdef __cplusplus
}
#endif

#endif
