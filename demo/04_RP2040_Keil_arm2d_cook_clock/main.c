#include "platform/pi_platform.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "service-cook-clock/arm_2d_scene_cook_clock.h"

int main(void)
{
    platform_init();
    arm_2d_init();
    disp_adapter0_init();
    arm_2d_scene_cook_clock_init(&DISP0_ADAPTER);

    while (true) {
        disp_adapter0_task();
    }
}

