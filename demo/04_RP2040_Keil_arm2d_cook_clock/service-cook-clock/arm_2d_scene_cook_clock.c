/*============================ INCLUDES ======================================*/

#define __USER_SCENE_COOK_CLOCK_IMPLEMENT__
#include "arm_2d_scene_cook_clock.h"

#if defined(RTE_Acceleration_Arm_2D_Helper_PFB)

#include <assert.h>
#include <string.h>
#include "pico/time.h"

/*============================ MACROS ========================================*/

#define COOK_CLOCK_COUNTDOWN_SECONDS        300
#define COOK_CLOCK_RING_DIAMETER             220
#define COOK_CLOCK_RING_RADIUS                (COOK_CLOCK_RING_DIAMETER / 2 - 6)
#define COOK_CLOCK_RING_WIDTH                 12
#define COOK_CLOCK_MICROSECONDS_PER_SECOND    1000000ULL
#define COOK_CLOCK_FULL_SWEEP_ANGLE_Q10          3600u

/*============================ MACROFIED FUNCTIONS ===========================*/

#undef this
#define this (*ptThis)

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ALARM_CLOCK_64_A4;

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_LiberationSansRegular32_A4;

static user_scene_cook_clock_t *s_ptCookClock;

/*============================ IMPLEMENTATION ================================*/

static void __cook_clock_update_ring(user_scene_cook_clock_t *ptThis)
{
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
        .hwColour = this.tDisplayColour.tValue,
        .iSweepAngle = 0,
        .iSweepAngleQ10 = (int16_t)this.hwSweepAngleQ10,
        .bRoundCaps = true,
    };
    arm_2d_err_t tResult = user_generic_loader_arc_set(&this.tCountdownRing,
                                                        &tArc);
    assert(ARM_2D_ERR_NONE == tResult);
    ARM_2D_UNUSED(tResult);
}

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
            arm_lcd_text_set_draw_region(&__centre_region);
            arm_lcd_text_set_colour(this.tDisplayColour.tValue,
                                    GLCD_COLOR_BLACK);
            if (this.bCountdownFinished &&
                (COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_BIRTHDAY ==
                 this.tCountdownFinishedEffect)) {
                arm_lcd_text_set_font(
                    (const arm_2d_font_t *)&ARM_2D_FONT_LiberationSansRegular32_A4);
                arm_lcd_printf_label(ARM_2D_ALIGN_CENTRE, "happy birthday");
            } else {
                arm_lcd_text_set_font(
                    (const arm_2d_font_t *)&ARM_2D_FONT_ALARM_CLOCK_64_A4);
                arm_lcd_printf_label(ARM_2D_ALIGN_CENTRE,
                                     "%02lu:%02lu",
                                     (unsigned long)(this.wSecondsRemaining / 60),
                                     (unsigned long)(this.wSecondsRemaining % 60));
            }
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
        .wSecondsRemaining = COOK_CLOCK_COUNTDOWN_SECONDS,
        .wCountdownDuration = COOK_CLOCK_COUNTDOWN_SECONDS,
        .qwCountdownStartTime = time_us_64(),
        .hwSweepAngleQ10 = COOK_CLOCK_FULL_SWEEP_ANGLE_Q10,
        .tCountdownFinishedEffect =
            COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_DEFAULT,
        .tDisplayColour = GLCD_COLOR_GREEN,
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
        .iSweepAngleQ10 = COOK_CLOCK_FULL_SWEEP_ANGLE_Q10,
        .bRoundCaps = true,
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

    s_ptCookClock = ptThis;

    return ptThis;
}

void __arm_2d_scene_cook_clock_set_colour(arm_2d_color_rgb565_t tColour)
{
    assert(NULL != s_ptCookClock);
    s_ptCookClock->tDisplayColour = tColour;
    __cook_clock_update_ring(s_ptCookClock);
}

void __arm_2d_scene_cook_clock_set_countdown(uint32_t wDurationInSeconds)
{
    assert(NULL != s_ptCookClock);

    s_ptCookClock->wSecondsRemaining = wDurationInSeconds;
    s_ptCookClock->wCountdownDuration = MAX(1u, wDurationInSeconds);
    s_ptCookClock->qwCountdownStartTime = time_us_64();
    s_ptCookClock->hwSweepAngleQ10 = COOK_CLOCK_FULL_SWEEP_ANGLE_Q10;
    s_ptCookClock->bCountdownFinished = false;
    __cook_clock_update_ring(s_ptCookClock);
}

void __arm_2d_scene_cook_clock_set_countdown_finished_effect(
                                    cook_clock_countdown_finished_effect_t tEffect)
{
    assert(NULL != s_ptCookClock);

    if (COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_BIRTHDAY != tEffect) {
        tEffect = COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_DEFAULT;
    }

    s_ptCookClock->tCountdownFinishedEffect = tEffect;
}

void __arm_2d_scene_cook_clock_set_countdown_finished_handler(
                                    cook_clock_countdown_finished_handler_t fnHandler,
                                    void *pTarget)
{
    assert(NULL != s_ptCookClock);

    s_ptCookClock->fnOnCountdownFinished = fnHandler;
    s_ptCookClock->pCountdownFinishedTarget = pTarget;
}

void __arm_2d_scene_cook_clock_task(void)
{
    assert(NULL != s_ptCookClock);

    uint64_t qwElapsedInMicroseconds =
        time_us_64() - s_ptCookClock->qwCountdownStartTime;
    uint64_t qwDurationInMicroseconds =
        (uint64_t)s_ptCookClock->wCountdownDuration
        * COOK_CLOCK_MICROSECONDS_PER_SECOND;
    uint32_t wSecondsRemaining;
    uint16_t hwSweepAngleQ10;

    if (qwElapsedInMicroseconds >= qwDurationInMicroseconds) {
        wSecondsRemaining = 0;
        hwSweepAngleQ10 = 0u;
    } else {
        wSecondsRemaining = s_ptCookClock->wCountdownDuration
            - (uint32_t)(qwElapsedInMicroseconds
                         / COOK_CLOCK_MICROSECONDS_PER_SECOND);
        hwSweepAngleQ10 = (uint16_t)(
            (((qwDurationInMicroseconds - qwElapsedInMicroseconds)
              * COOK_CLOCK_FULL_SWEEP_ANGLE_Q10)
             + (qwDurationInMicroseconds / 2u)) /
            qwDurationInMicroseconds);
    }

    if ((wSecondsRemaining != s_ptCookClock->wSecondsRemaining) ||
        (hwSweepAngleQ10 != s_ptCookClock->hwSweepAngleQ10)) {
        bool const bCountdownFinished = (0u == wSecondsRemaining) &&
                                      (0u != s_ptCookClock->wSecondsRemaining);

        s_ptCookClock->wSecondsRemaining = wSecondsRemaining;
        s_ptCookClock->hwSweepAngleQ10 = hwSweepAngleQ10;
        s_ptCookClock->bCountdownFinished = bCountdownFinished;
        __cook_clock_update_ring(s_ptCookClock);

        if (bCountdownFinished &&
            (NULL != s_ptCookClock->fnOnCountdownFinished)) {
            s_ptCookClock->fnOnCountdownFinished(
                                    s_ptCookClock->tCountdownFinishedEffect,
                                    s_ptCookClock->pCountdownFinishedTarget);
        }
    }
}

#endif