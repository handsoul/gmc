#include "SysCfg.h"
#include "DataType.h"
#include <string.h>


// 速度代码.
const s32 g_aslSpdTable[SPEED_MAX_INDX] = 
{
    0,
    10,
    20,
    30,
    40,
    50,
    60,
    70,
    80,
};

// 位置代码.
POS_NAME_ST g_astPosTable[] = 
{
    {"MA" , -50},
    {"MB" , -40},
    {"MC" , -30},
    {"MD" , -20},
    {"ME" , -10},
    {"M1" ,   0},
    {"M2" ,  10},
    {"M3" ,  20},
    {"M4" ,  30}, 
    {"M5" ,  40},
    {"M6" ,  50},
    {"M7" ,  60},
    {"M8" ,  70},
    {"M9" ,  80},
    {"M10",  90},
};

// 信号名代码. O
POS_NAME_ST g_astSigTable[] = 
{
    {"MA" , 0},
    {"MB" , 1},
    {"MC" , 2},
    {"MD" , 3},
    {"ME" , 4},
    {"M1" , 5},
    {"M2" , 6},
    {"M3" , 7},
    {"M4" , 8}, 
    {"M5" , 9},
    {"M6" , 10},
    {"M7" , 11},
    {"M8" , 12},
    {"M9" , 13},
    {"M10", 14},
};


s32 GetSigIndexByName(const char * pSigName)
{
    int i  = 0;
    for(;i < (int)ITEMS_OF(g_astPosTable);i++)
    {
        if (memcmp(g_astPosTable[i].m_pscName,pSigName,strlen(g_astPosTable[i].m_pscName)) == 0)
        {
            return i;
        }
    }

    return -1;
}

bool GetPosValueByName(const char * pPosName, s32 *pslPosValue)
{
    int i  = 0;
    for(;i < (int)ITEMS_OF(g_astPosTable);i++)
    {
        if (memcmp(g_astPosTable[i].m_pscName,pPosName,strlen(g_astPosTable[i].m_pscName)) == 0)
        {
            *pslPosValue = g_astPosTable[i].m_slPos;
            return true;
        }
    }

    return false;
}
