#ifndef __SYSCFG_H__
#define __SYSCFG_H__
#include "DataType.h"

typedef struct tagPosNameValue
{
    const char * m_pscName; // 位置名称.
    s32 m_slPos;        // 位置
}POS_NAME_ST;

// 速度-索引表.
typedef enum tagEnumSpd
{
    SPD_0 = 0,
    SPD_1,
    SPD_2,
    SPD_3,
    SPD_4,
    SPD_5,
    SPD_7,
    SPD_8,
    SPD_9,

    SPEED_MAX_INDX
}E_SPEED_INDX;

extern const s32 g_aslSpdTable[SPEED_MAX_INDX];
extern s32 GetPosIndexByName(const char * pSigName);
extern bool GetPosValueByName(const char * pPosName, s32 *pPosValue);
extern s32 GetSigIndexByName(const char * pSigName);

#endif
