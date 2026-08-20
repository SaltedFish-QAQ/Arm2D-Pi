/*============================ INCLUDES ======================================*/

#define __USER_SCENE_COOK_CLOCK_IMPLEMENT__
#include "arm_2d_scene_cook_clock.h"

#if defined(RTE_Acceleration_Arm_2D_Helper_PFB)

#include <assert.h>
#include <string.h>

/*============================ MACROS ========================================*/

#define COOK_CLOCK_COUNTDOWN_SECONDS        300
#define COOK_CLOCK_RING_DIAMETER             220
#define COOK_CLOCK_RING_RADIUS                (COOK_CLOCK_RING_DIAMETER / 2 - 6)
#define COOK_CLOCK_RING_WIDTH                 12

/*============================ MACROFIED FUNCTIONS ===========================*/

#undef this
#define this (*ptThis)

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ALARM_CLOCK_64_A4;

/*============================ IMPLEMENTATION ================================*/

static void __on_scene_cook_clock_load(arm_2d_scene_t *ptScene)
{
    user_scene_cook_clock_t *ptThis = (user_scene_cook_clock_t *)ptScene;

    user_generic_loader_arc_on_load(&this.tCountdownRing);
}

static void __on_scene_cook_clock_depose(arm_2d_scene_t *ptScene)
{
    user_scene_cook_clock_t *ptThis = (user_scene_cook_clock_t *)ptScene;

    user_generic_loader_arc_depose(&this.tCountdownRing);
    ptScene->ptPlayer = NULL;
    if (!this.bUserAllocated) {
        __arm_2d_free_scratch_memory(ARM_2D_MEM_TYPE_UNSPECIFIED, ptScene);
    }
}

static void __on_scene_cook_clock_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_cook_clock_t *ptThis = (user_scene_cook_clock_t *)ptScene;

    if (arm_2d_helper_is_time_out(1000, &this.lTimestamp)) {
        this.lTimestamp = 0;
        if (this.hwSecondsRemaining > 0) {
            this.hwSecondsRemaining--;
        }
    }

    user_generic_loader_arc_param_t tArc = {
        .tCenter = {
            .iX = __DISP0_CFG_SCEEN_WIDTH__ / 2,
            .iY = __DISP0_CFG_SCEEN_HEIGHT__ / 2,
        },
        .tStartPoint = {
            .iX = __DISP0_CFG_SCEEN_WIDTH__ / 2,
            .iY = __DISP0_CFG_SCEEN_HEIGHT__ / 2 - COOK_CLOCK_RING_RADIUS,
        },
        .hwRadius = COOK_CLOCK_RING_RADIUS,
        .hwRingWidth = COOK_CLOCK_RING_WIDTH,
        .hwColour = GLCD_COLOR_GREEN,
        .iSweepAngle = (int16_t)(360UL * this.hwSecondsRemaining
                               / COOK_CLOCK_COUNTDOWN_SECONDS),
    };
    (void)user_generic_loader_arc_set(&this.tCountdownRing, &tArc);
    user_generic_loader_arc_on_frame_start(&this.tCountdownRing);
}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene_cook_clock_handler)
{
    user_scene_cook_clock_t *ptThis = (user_scene_cook_clock_t *)pTarget;

    ARM_2D_PARAM(bIsNewFrame);

    arm_2d_canvas(ptTile, __canvas) {
        user_generic_loader_arc_show(&this.tCountdownRing,
                                     ptTile,
                                     &__canvas,
                                     bIsNewFrame);

        arm_2d_align_centre(__canvas, 240, 80) {
            arm_lcd_text_set_target_framebuffer((arm_2d_tile_t *)ptTile);
            arm_lcd_text_set_font(
                (const arm_2d_font_t *)&ARM_2D_FONT_ALARM_CLOCK_64_A4);
            arm_lcd_text_set_draw_region(&__centre_region);
            arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_BLACK);
            arm_lcd_printf_label(ARM_2D_ALIGN_CENTRE,
                                 "%02u:%02u",
                                 this.hwSecondsRemaining / 60,
                                 this.hwSecondsRemaining % 60);
            arm_lcd_text_set_target_framebuffer(NULL);
        }
    }

    ARM_2D_OP_WAIT_ASYNC();
    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_cook_clock_t *__arm_2d_scene_cook_clock_init(
                                    arm_2d_scene_player_t *ptDispAdapter,
                                    user_scene_cook_clock_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

    if (NULL == ptThis) {
        ptThis = (user_scene_cook_clock_t *)
                    __arm_2d_allocate_scratch_memory(
                                    sizeof(user_scene_cook_clock_t),
                                    __alignof__(user_scene_cook_clock_t),
                                    ARM_2D_MEM_TYPE_UNSPECIFIED);
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
        memset(ptThis, 0, sizeof(user_scene_cook_clock_t));
    }

    *ptThis = (user_scene_cook_clock_t){
        .use_as__arm_2d_scene_t = {
            .tCanvas = {GLCD_COLOR_BLACK},
            .fnOnLoad = &__on_scene_cook_clock_load,
            .fnScene = &__pfb_draw_scene_cook_clock_handler,
            .fnOnFrameStart = &__on_scene_cook_clock_frame_start,
            .fnDepose = &__on_scene_cook_clock_depose,
        },
        .bUserAllocated = bUserAllocated,
        .hwSecondsRemaining = COOK_CLOCK_COUNTDOWN_SECONDS,
    };

    user_generic_loader_arc_param_t tArc = {
        .tCenter = {
            .iX = __DISP0_CFG_SCEEN_WIDTH__ / 2,
            .iY = __DISP0_CFG_SCEEN_HEIGHT__ / 2,
        },
        .tStartPoint = {
            .iX = __DISP0_CFG_SCEEN_WIDTH__ / 2,
            .iY = __DISP0_CFG_SCEEN_HEIGHT__ / 2 - COOK_CLOCK_RING_RADIUS,
        },
        .hwRadius = COOK_CLOCK_RING_RADIUS,
        .hwRingWidth = COOK_CLOCK_RING_WIDTH,
        .hwColour = GLCD_COLOR_GREEN,
        .iSweepAngle = 360,
    };
    user_generic_loader_arc_cfg_t tArcCFG = {
        .tSize = {
            .iWidth = __DISP0_CFG_SCEEN_WIDTH__,
            .iHeight = __DISP0_CFG_SCEEN_HEIGHT__,
        },
        .ptScene = &this.use_as__arm_2d_scene_t,
        .tArc = tArc,
    };
    arm_2d_err_t tResult = user_generic_loader_arc_init(
                                &this.tCountdownRing,
                                &tArcCFG);
    assert(ARM_2D_ERR_NONE == tResult);
    ARM_2D_UNUSED(tResult);

    arm_2d_scene_player_append_scenes(ptDispAdapter,
                                      &this.use_as__arm_2d_scene_t,
                                      1);

    return ptThis;
}

#endif