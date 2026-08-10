#ifndef __PAGE1_LOGIC_H__
#define __PAGE1_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void page1LogicInit(ld_scene_t* ptScene);
void page1LogicFrameStart(ld_scene_t* ptScene);
void page1LogicLoop(ld_scene_t* ptScene);
void page1LogicDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame);
void page1LogicFrameComplete(ld_scene_t* ptScene);
void page1LogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
