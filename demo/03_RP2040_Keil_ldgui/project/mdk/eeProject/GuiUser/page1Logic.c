#include "page1Logic.h"
#include "page1.h"
#include "ldGui.h"
#include "home.h"

void page1LogicInit(ld_scene_t* ptScene)
{

}

void page1LogicFrameStart(ld_scene_t* ptScene)
{

}

void page1LogicLoop(ld_scene_t* ptScene)
{

}

void page1LogicDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

}

void page1LogicFrameComplete(ld_scene_t* ptScene)
{

}

void page1LogicQuit(ld_scene_t* ptScene)
{

}

void page1_Timer_jump_event(ld_scene_t* ptScene)
{
    ldGuiJumpPage(homeFunc);
}



void page1_Timer_run_event(ld_scene_t* ptScene)
{
    ldRadialMenu_t *obj=ldBaseGetWidgetById(ID_RADIALMENU_0);
    ldRadialMenuSetOffsetItem(obj,1);
}
