#include "home.h"
#include "homeLogic.h"
#include "ldGui.h"

void homeInit(ld_scene_t* ptScene);
void homeFrameStart(ld_scene_t* ptScene);
void homeLoop(ld_scene_t* ptScene);
void homeDraw(ld_scene_t* ptScene, arm_2d_tile_t *ptTile, bool bIsNewFrame);
void homeFrameComplete(ld_scene_t* ptScene);
void homeQuit(ld_scene_t* ptScene);

const ldPageFuncGroup_t homeFunc={
    .init=homeInit,
    .frameStart=homeFrameStart,
    .loop=homeLoop,
    .draw=homeDraw,
    .frameComplete=homeFrameComplete,
    .quit=homeQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="home",
#endif
};

#if 1==1
const uint8_t home_Text_0_staticText[]="LDGUI";
#endif

ldTimer_t home_Timer_0=0;
ldTimer_t home_Timer_jump=0;


__WEAK void home_Timer_0_event(ld_scene_t* ptScene)
{
}

__WEAK void home_Timer_jump_event(ld_scene_t* ptScene)
{
}



void homeInit(ld_scene_t* ptScene)
{
    void *obj;
    obj=ldWindowInit(ID_BACKGROUND_0, ID_BACKGROUND_0, 0, 0, 320, 240);
#if 0==0
    ldWindowSetColor(obj, __RGB(0xFE, 0xFE, 0xFE));
#endif
#if 0==1
    ldWindowSetImage(obj, IMAGE_, IMAGE__MASK);
#endif

    obj=ldTextInit(ID_TEXT_0, ID_BACKGROUND_0, 0, 10, 320, 50, FONT_SOURCE_HAN_SANS_SC_NORMAL_22, 2, 0);
#if 1==0
    ldTextSetText(obj, (uint8_t*)"LDGUI");
#endif
#if 1==1
    ldTextSetStaticText(obj, home_Text_0_staticText);
#endif
    ldTextSetTextColor(obj, __RGB(0x00, 0x00, 0x00));
#if 1==0
    ldTextSetTransparent(obj, true);
#endif
#if 1==1
    ldTextSetBackgroundColor(obj, __RGB(0xFF, 0xFF, 0xFF));
#endif
#if 1==2
    ldTextSetBackgroundImage(obj, IMAGE_, IMAGE__MASK);
#endif

    obj=ldProgressBarInit(ID_PROGRESSBAR_0, ID_BACKGROUND_0, 60, 190, 204, 20);
#if 0==0
    ldProgressBarSetColor(obj, __RGB(0xE6, 0xE6, 0xE6), __RGB(0x06, 0xB0, 0x25));
#if 1!=0
    ldProgressBarSetFrameColor(obj, __RGB(0xBC, 0xBC, 0xBC), 1);
#endif
#endif
#if 0==1
    ldProgressBarSetImage(obj, IMAGE_, IMAGE__MASK, IMAGE_, IMAGE__MASK);
#ifdef IMAGE_
    ldProgressBarSetFrameImage(obj, IMAGE_, IMAGE__MASK);
#endif
#endif
    ldProgressBarSetHorizontal(obj, 1);
    ldProgressBarSetPercent(obj, (42-0)*100/(100-0));

    home_Timer_0=0;
    obj=ldQRCodeInit(ID_QRCODE_0, ID_BACKGROUND_0, 50, 70, 66, 66, (uint8_t*)"https://gitee.com/gzbkey/LingDongGUI", 4278190080, 4294967295, 1, 4, 2);

    obj=ldLabelInit(ID_LABEL_0, ID_BACKGROUND_0, 50, 150, 60, 30, FONT_SOURCE_HAN_SANS_SC_NORMAL_16);
    ldLabelSetText(obj, (uint8_t*)"介绍");
    ldLabelSetTextColor(obj, __RGB(0x00, 0x00, 0x00));
#if 1==0
    ldLabelSetTransparent(obj, true);
#endif
#if 1==1
    ldLabelSetBackgroundColor(obj, __RGB(0xFF, 0xFF, 0xFF));
#endif
#if 1==2
    ldLabelSetBackgroundImage(obj, IMAGE_, IMAGE__MASK);
#endif
#if 2==0
#define LABEL_0_ALIGN_H ARM_2D_ALIGN_LEFT
#endif
#if 2==1
#define LABEL_0_ALIGN_H ARM_2D_ALIGN_RIGHT
#endif
#if 2==2
#define LABEL_0_ALIGN_H ARM_2D_ALIGN_CENTRE
#endif
#if 2==0
#define LABEL_0_ALIGN_V ARM_2D_ALIGN_TOP
#endif
#if 2==1
#define LABEL_0_ALIGN_V ARM_2D_ALIGN_BOTTOM
#endif
#if 2==2
#define LABEL_0_ALIGN_V ARM_2D_ALIGN_CENTRE
#endif
    ldLabelSetAlign(obj, LABEL_0_ALIGN_H|LABEL_0_ALIGN_V);

    obj=ldQRCodeInit(ID_QRCODE_1, ID_BACKGROUND_0, 200, 70, 66, 66, (uint8_t*)"https://gitee.com/gzbkey/GuiEasyEditor", 4278190080, 4294967295, 1, 4, 2);

    obj=ldLabelInit(ID_LABEL_1, ID_BACKGROUND_0, 200, 150, 60, 30, FONT_SOURCE_HAN_SANS_SC_NORMAL_16);
    ldLabelSetText(obj, (uint8_t*)"上位机");
    ldLabelSetTextColor(obj, __RGB(0x00, 0x00, 0x00));
#if 1==0
    ldLabelSetTransparent(obj, true);
#endif
#if 1==1
    ldLabelSetBackgroundColor(obj, __RGB(0xFF, 0xFF, 0xFF));
#endif
#if 1==2
    ldLabelSetBackgroundImage(obj, IMAGE_, IMAGE__MASK);
#endif
#if 2==0
#define LABEL_1_ALIGN_H ARM_2D_ALIGN_LEFT
#endif
#if 2==1
#define LABEL_1_ALIGN_H ARM_2D_ALIGN_RIGHT
#endif
#if 2==2
#define LABEL_1_ALIGN_H ARM_2D_ALIGN_CENTRE
#endif
#if 2==0
#define LABEL_1_ALIGN_V ARM_2D_ALIGN_TOP
#endif
#if 2==1
#define LABEL_1_ALIGN_V ARM_2D_ALIGN_BOTTOM
#endif
#if 2==2
#define LABEL_1_ALIGN_V ARM_2D_ALIGN_CENTRE
#endif
    ldLabelSetAlign(obj, LABEL_1_ALIGN_H|LABEL_1_ALIGN_V);

    home_Timer_jump=0;
    

    homeLogicInit(ptScene);
}

void homeFrameStart(ld_scene_t* ptScene)
{

    homeLogicFrameStart(ptScene);
}

void homeLoop(ld_scene_t* ptScene)
{
    if(ldTimeOut(500,1,&home_Timer_0))
    {
        home_Timer_0_event(ptScene);
    }

    if(ldTimeOut(10000,1,&home_Timer_jump))
    {
        home_Timer_jump_event(ptScene);
    }

    

    homeLogicLoop(ptScene);
}

void homeDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

    homeLogicDraw(ptScene, ptTile, bIsNewFrame);
}

void homeFrameComplete(ld_scene_t* ptScene)
{

    homeLogicFrameComplete(ptScene);
}

void homeQuit(ld_scene_t* ptScene)
{

    homeLogicQuit(ptScene);
}
