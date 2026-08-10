#include "page1.h"
#include "page1Logic.h"
#include "ldGui.h"

void page1Init(ld_scene_t* ptScene);
void page1FrameStart(ld_scene_t* ptScene);
void page1Loop(ld_scene_t* ptScene);
void page1Draw(ld_scene_t* ptScene, arm_2d_tile_t *ptTile, bool bIsNewFrame);
void page1FrameComplete(ld_scene_t* ptScene);
void page1Quit(ld_scene_t* ptScene);

const ldPageFuncGroup_t page1Func={
    .init=page1Init,
    .frameStart=page1FrameStart,
    .loop=page1Loop,
    .draw=page1Draw,
    .frameComplete=page1FrameComplete,
    .quit=page1Quit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="page1",
#endif
};

ldTimer_t page1_Timer_jump=0;
ldTimer_t page1_Timer_run=0;


__WEAK void page1_Timer_jump_event(ld_scene_t* ptScene)
{
}

__WEAK void page1_Timer_run_event(ld_scene_t* ptScene)
{
}



void page1Init(ld_scene_t* ptScene)
{
    void *obj;
    obj=ldWindowInit(ID_BACKGROUND_0, ID_BACKGROUND_0, 0, 0, 320, 240);
#if 0==0
    ldWindowSetColor(obj, __RGB(0xF0, 0xF0, 0xF0));
#endif
#if 0==1
    ldWindowSetImage(obj, IMAGE_, IMAGE__MASK);
#endif

    page1_Timer_jump=0;
    obj=ldRadialMenuInit(ID_RADIALMENU_0, ID_BACKGROUND_0, 40, 50, 250, 150, 140, 80, 4);
    ldRadialMenuAddItem(obj, IMAGE_MANAGEMENT_64_BMP, IMAGE_MANAGEMENT_64_BMP_MASK);
    ldRadialMenuAddItem(obj, IMAGE_CLOCK_64_BMP, IMAGE_CLOCK_64_BMP_MASK);
    ldRadialMenuAddItem(obj, IMAGE_CALENDAR_64_BMP, IMAGE_CALENDAR_64_BMP_MASK);
    ldRadialMenuAddItem(obj, IMAGE_SCHEDULE_64_BMP, IMAGE_SCHEDULE_64_BMP_MASK);

    page1_Timer_run=0;
    

    page1LogicInit(ptScene);
}

void page1FrameStart(ld_scene_t* ptScene)
{

    page1LogicFrameStart(ptScene);
}

void page1Loop(ld_scene_t* ptScene)
{
    if(ldTimeOut(15000,1,&page1_Timer_jump))
    {
        page1_Timer_jump_event(ptScene);
    }

    if(ldTimeOut(3000,1,&page1_Timer_run))
    {
        page1_Timer_run_event(ptScene);
    }

    

    page1LogicLoop(ptScene);
}

void page1Draw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

    page1LogicDraw(ptScene, ptTile, bIsNewFrame);
}

void page1FrameComplete(ld_scene_t* ptScene)
{

    page1LogicFrameComplete(ptScene);
}

void page1Quit(ld_scene_t* ptScene)
{

    page1LogicQuit(ptScene);
}
