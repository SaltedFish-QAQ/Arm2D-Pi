#include "homeLogic.h"
#include "home.h"
#include "ldGui.h"
#include "page1.h"

void homeLogicInit(ld_scene_t* ptScene)
{

}

void homeLogicFrameStart(ld_scene_t* ptScene)
{

}

void homeLogicLoop(ld_scene_t* ptScene)
{

}

void homeLogicDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

}

void homeLogicFrameComplete(ld_scene_t* ptScene)
{

}

void homeLogicQuit(ld_scene_t* ptScene)
{

}

void home_Timer_0_event(ld_scene_t* ptScene)
{
    static uint8_t s_percent = 0;
    s_percent += 10;
    if (s_percent > 100) {
        s_percent = 0;
    }
    ldProgressBarSetPercent(ldBaseGetWidgetById(ID_PROGRESSBAR_0), (float)s_percent);
}

void home_Timer_jump_event(ld_scene_t* ptScene)
{
    ldGuiJumpPage(page1Func);
}

