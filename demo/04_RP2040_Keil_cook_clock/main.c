#include "platform/pi_platform.h"
#include <stdio.h>
#include "ldgui.h"
#include "home.h"

static void system_init(void)
{
    platform_init();
}

int main(void) 
{
    platform_init();
    sleep_ms(500);

    ldGuiInit((ldPageFuncGroup_t *)&homeFunc);
    
    while(1)
    {
        ldGuiLoop();
    }
}

