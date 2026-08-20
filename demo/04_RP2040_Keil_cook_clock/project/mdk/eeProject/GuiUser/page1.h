#ifndef __PAGE1_H__
#define __PAGE1_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldBase.h"
#if USE_VIRTUAL_RESOURCE == 0
#include "fonts/uiFonts.h"
#include "images/uiImages.h"
#else
#include "binData.h"
#endif

extern const ldPageFuncGroup_t page1Func;

#define ID_BACKGROUND_0                 0
#define ID_RADIALMENU_0                 2


#ifdef __cplusplus
}
#endif

#endif
