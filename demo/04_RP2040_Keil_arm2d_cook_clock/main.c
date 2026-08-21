#include "platform/pi_platform.h"
#include "platform/st7789_simple.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "application/buzzer_task.h"
#include "application/power_task.h"
#include "application/qmi8658c_task.h"
#include "service-cook-clock/arm_2d_scene_cook_clock.h"
#include "hardware/sync.h"

#ifndef __COOK_CLOCK_ENABLE_AUTOMATED_TEST__
#   define __COOK_CLOCK_ENABLE_AUTOMATED_TEST__     0
#endif

#define COOK_CLOCK_INITIAL_DURATION_IN_SECONDS       (5u * 60u)
#define COOK_CLOCK_INITIAL_COLOUR                     ((arm_2d_color_rgb565_t){.tValue = __RGB(0, 255, 0)})

#if __COOK_CLOCK_ENABLE_AUTOMATED_TEST__
typedef struct cook_clock_test_step_t {
    uint8_t chDurationInSeconds;
    arm_2d_color_rgb565_t tColour;
} cook_clock_test_step_t;

static const cook_clock_test_step_t c_tCookClockTestSteps[] = {
    {10, {.tValue = __RGB(255,   0,   0)}},
    {20, {.tValue = __RGB(255, 127,   0)}},
    {30, {.tValue = __RGB(255, 255,   0)}},
    {40, {.tValue = __RGB(  0, 255,   0)}},
    {50, {.tValue = __RGB(  0, 255, 255)}},
    {60, {.tValue = __RGB(  0,   0, 255)}},
    {70, {.tValue = __RGB(127,   0, 255)}},
};

typedef struct cook_clock_test_t {
    uint64_t qwStepStartTime;
    uint8_t chStep;
    bool bWaitingForAlert;
} cook_clock_test_t;

static cook_clock_test_t s_tCookClockTest;

static void __cook_clock_test_apply_step(cook_clock_test_t *ptThis)
{
    const cook_clock_test_step_t *ptStep =
        &c_tCookClockTestSteps[ptThis->chStep];

    cook_clock_set_colour(ptStep->tColour);
    cook_clock_set_countdown(ptStep->chDurationInSeconds);
    ptThis->qwStepStartTime = time_us_64();
    ptThis->bWaitingForAlert = false;
}

static void __cook_clock_test_init(void)
{
    s_tCookClockTest.chStep = 0;
    __cook_clock_test_apply_step(&s_tCookClockTest);
}

static void __cook_clock_test_task(void)
{
    cook_clock_test_t *ptThis = &s_tCookClockTest;
    const cook_clock_test_step_t *ptStep =
        &c_tCookClockTestSteps[ptThis->chStep];

    if (!ptThis->bWaitingForAlert &&
        ((time_us_64() - ptThis->qwStepStartTime)
        >= (uint64_t)ptStep->chDurationInSeconds * 1000000ULL)) {
        ptThis->bWaitingForAlert = true;
    }

    if (ptThis->bWaitingForAlert && !app_buzzer_is_playing()) {
        ptThis->chStep = (ptThis->chStep + 1u) % dimof(c_tCookClockTestSteps);
        __cook_clock_test_apply_step(ptThis);
    }
}
#endif

static void __on_cook_clock_countdown_finished(
                                cook_clock_countdown_finished_effect_t tEffect,
                                void *pTarget)
{
    (void)pTarget;

    if (COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_BIRTHDAY == tEffect) {
        (void)app_buzzer_play_happy_birthday();
    } else {
        (void)app_buzzer_play_countdown_finished_chime();
    }
}

static void __cook_clock_display_init(void)
{
    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();
    arm_2d_scene_cook_clock_init(&DISP0_ADAPTER);
    // COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_BIRTHDAY
    // COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_DEFAULT
    cook_clock_set_countdown_finished_effect(
                                COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_DEFAULT);
    cook_clock_set_countdown_finished_handler(
                                __on_cook_clock_countdown_finished, NULL);
    cook_clock_set_colour(COOK_CLOCK_INITIAL_COLOUR);
    cook_clock_set_countdown(COOK_CLOCK_INITIAL_DURATION_IN_SECONDS);

#if __COOK_CLOCK_ENABLE_AUTOMATED_TEST__
    __cook_clock_test_init();
#endif
}

int main(void)
{
    platform_init();
    st7789_set_backlight(false);
    buzzer_task_init();
    power_task_init();
    (void)qmi8658c_init();
    __cook_clock_display_init();

    while (arm_fsm_rt_cpl != disp_adapter0_task()) {
    }

    st7789_set_backlight(true);

    while (true) {
        uint32_t wNowMS = to_ms_since_boot(get_absolute_time());

        buzzer_task(wNowMS);
        power_task(wNowMS);
        qmi8658c_task(wNowMS);
        cook_clock_task();
#if __COOK_CLOCK_ENABLE_AUTOMATED_TEST__
        __cook_clock_test_task();
#endif
        disp_adapter0_task();
    }
}

