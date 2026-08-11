

/* generated with ttf2c.py (v2.4.1) */

#include "arm_2d_helper.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wgnu-variable-sized-type-not-at-end"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#elif defined(__IS_COMPILER_GCC__)
#   pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress=1296,1,64,177
#endif

typedef struct {
    uint16_t      hwIndex;
    arm_2d_size_t tCharSize;
    int16_t       iAdvance;
    int16_t       iBearingX;
    int16_t       iBearingY;
    uint8_t       chCodeLength;
    uint8_t       chUTF8[4];
} const __ttf_char_descriptor_t;

#ifndef XIP_BASE
#define XIP_BASE ((uint8_t *)0x00000000)
#endif

#ifndef FONT_SimSun_9_A8_XIP_OFFSET
#define FONT_SimSun_9_A8_XIP_OFFSET 0x00000BBA
#endif

ARM_SECTION("arm2d.asset.FONT.c_bmpUTF8SimSun_9A8Font")
const uint8_t c_bmpUTF8SimSun_9A8Font[] = {

    // a white space
    0x00, 0x00, 0x00, 0x00, 
};

ARM_SECTION("arm2d.asset.FONT.c_tUTF8SimSun_9LookUpTableA8")
 const __ttf_char_descriptor_t c_tUTF8SimSun_9LookUpTableA8[1] = {

    { 0, { 2, 2, }, 1, 0, 2, 1, {0x20} },
};



ARM_SECTION("arm2d.tile.c_tileUTF8SimSun_9A8Mask")
static const arm_2d_tile_t c_tileUTF8SimSun_9A8Mask = {
    .tRegion = {
        .tSize = {
            .iWidth = 2,
            .iHeight = 3,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + FONT_SimSun_9_A8_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_bmpUTF8SimSun_9A8Font,
#endif
};

#define __UTF8_FONT_SIZE_8__



static
IMPL_FONT_GET_CHAR_DESCRIPTOR(__utf8_a8_font_get_char_descriptor)
{
    assert(NULL != ptFont);
    assert(NULL != ptDescriptor);
    assert(NULL != pchCharCode);

    arm_2d_user_font_t *ptThis = (arm_2d_user_font_t *)ptFont;
    ARM_2D_UNUSED(ptThis);

    memset(ptDescriptor, 0, sizeof(arm_2d_char_descriptor_t));

    ptDescriptor->tileChar.ptParent = (arm_2d_tile_t *)&ptFont->tileFont;
    ptDescriptor->tileChar.tInfo.bDerivedResource = true;

    /* use the white space as the default char */
    __ttf_char_descriptor_t *ptUTF8Char =
        (__ttf_char_descriptor_t *)&c_tUTF8SimSun_9LookUpTableA8[dimof(c_tUTF8SimSun_9LookUpTableA8)-1];

    arm_foreach(__ttf_char_descriptor_t, c_tUTF8SimSun_9LookUpTableA8, ptChar) {
        if (0 == strncmp(   (char *)pchCharCode,
                            (char *)ptChar->chUTF8,
                            ptChar->chCodeLength)) {
            /* found the UTF8 char */
            ptUTF8Char = ptChar;
            break;
        }
    }

    ptDescriptor->chCodeLength = ptUTF8Char->chCodeLength;
    ptDescriptor->tileChar.tRegion.tSize = ptUTF8Char->tCharSize;
    ptDescriptor->tileChar.tRegion.tLocation.iY = (int16_t)ptUTF8Char->hwIndex;

    ptDescriptor->iAdvance = ptUTF8Char->iAdvance;
    ptDescriptor->iBearingX= ptUTF8Char->iBearingX;
    ptDescriptor->iBearingY= ptUTF8Char->iBearingY;

    return ptDescriptor;
}

ARM_SECTION("arm2d.asset.FONT.ARM_2D_FONT_SimSun_9_A8")
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_SimSun_9_A8 = {

    .use_as__arm_2d_user_font_t = {
        .use_as__arm_2d_font_t = {
            .tileFont = impl_child_tile(
                c_tileUTF8SimSun_9A8Mask,
                0,          /* x offset */
                0,          /* y offset */
                2,        /* width */
                3         /* height */
            ),
            .tCharSize = {
                .iWidth = 2,
                .iHeight = 2,
            },
            .nCount =  0,                             //!< Character count
            .fnGetCharDescriptor = &__utf8_a8_font_get_char_descriptor,
            .fnDrawChar = &__arm_2d_lcd_text_default_a8_font_draw_char,
        },
        .hwCount = 1,
        .hwDefaultCharIndex = 1, /* tBlank */
    },

    .tUTF8Table = {
        .hwCount = 0,
        .hwOffset = 0,
    },
};

#undef __UTF8_FONT_SIZE_8__



#if defined(__clang__)
#   pragma clang diagnostic pop
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_warning=1296
#endif

