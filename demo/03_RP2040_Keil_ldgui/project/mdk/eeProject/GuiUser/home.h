#ifndef __HOME_H__
#define __HOME_H__

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

extern const ldPageFuncGroup_t homeFunc;

#define ID_BACKGROUND_0                 0
#define ID_TEXT_0                 1
#define ID_PROGRESSBAR_0                 2
#define ID_QRCODE_0                 4
#define ID_LABEL_0                 5
#define ID_QRCODE_1                 6
#define ID_LABEL_1                 7


#ifdef __cplusplus
}
#endif

#endif
