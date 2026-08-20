/*============================ INCLUDES ======================================*/

#ifndef __ARM_2D_SCENE_COOK_CLOCK_H__
#define __ARM_2D_SCENE_COOK_CLOCK_H__

#include "arm_2d.h"

#if defined(RTE_Acceleration_Arm_2D_Helper_PFB)

#include "arm_2d_helper.h"
#include "arm_2d_helper_scene.h"
#include "arm_2d_example_controls.h"
#include "../../02_RP2040_Keil-arm2d_3d/3d/user_generic_loader_arc.h"

#ifdef   __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/

#ifdef __USER_SCENE_COOK_CLOCK_IMPLEMENT__
#   undef __USER_SCENE_COOK_CLOCK_IMPLEMENT__
#   define __ARM_2D_IMPL__
#endif
#include "arm_2d_utils.h"

/*============================ MACROFIED FUNCTIONS ===========================*/

#define arm_2d_scene_cook_clock_init(__DISP_ADAPTER_PTR, ...)                  \
            __arm_2d_scene_cook_clock_init((__DISP_ADAPTER_PTR), (NULL, ##__VA_ARGS__))

#define cook_clock_set_colour(__COLOUR)                                         \
            __arm_2d_scene_cook_clock_set_colour((__COLOUR))

#define cook_clock_set_countdown(__DURATION_IN_SECONDS)                         \
            __arm_2d_scene_cook_clock_set_countdown((__DURATION_IN_SECONDS))

#define cook_clock_task()                                                        \
            __arm_2d_scene_cook_clock_task()

/*============================ TYPES =========================================*/

typedef struct user_scene_cook_clock_t user_scene_cook_clock_t;

struct user_scene_cook_clock_t {
    implement(arm_2d_scene_t);

ARM_PRIVATE(
    bool bUserAllocated;
    uint32_t wSecondsRemaining;
    uint32_t wCountdownDuration;
    uint64_t qwCountdownStartTime;
    arm_2d_color_rgb565_t tDisplayColour;
    user_generic_loader_arc_t tCountdownRing;
)
};

/*============================ PROTOTYPES ====================================*/

ARM_NONNULL(1)
extern
user_scene_cook_clock_t *__arm_2d_scene_cook_clock_init(
                                    arm_2d_scene_player_t *ptDispAdapter,
                                    user_scene_cook_clock_t *ptScene);

extern
void __arm_2d_scene_cook_clock_set_colour(arm_2d_color_rgb565_t tColour);

extern
void __arm_2d_scene_cook_clock_set_countdown(uint32_t wDurationInSeconds);

extern
void __arm_2d_scene_cook_clock_task(void);

#ifdef   __cplusplus
}
#endif

#endif

#endif