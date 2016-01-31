#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "Parser.h"

CMD_ST g_stCmd = 
{
    .m_ucLen = 0,
};

RUN_STATE_ST g_stRunState;


#define IS_DIGITAL(c) \
    ((c > '0') && (c < '9'))

bool IsValidSigName(char * pSigName)
{
    if ((*pSigName != 'M' 
        && *pSigName != 'H'))
    {
        return false;
    }

    return true;
}

// 位置解析.
// 返回值: 解析成功/错误.
// 完成绝对位置/相对位置的解析. 固定位置-配置.
bool HfnGetPos( char * pCmdStr, CONDITION_POS_ST * pCondPos)
{
    return true;
}

// 信号名称解析.
// 返回值: 解析成功/错误.
// 完成信号名 - 信号资源的映射.
bool HfnGetSig(char * pCmdStr, CONDITION_SIG_ST * pCondSig)
{
    return true;
}

// 动作解析
bool HfnGetAction(char *pCmdStr, ACTION_ST * pActionSt)
{
    return true;
}

/*设置运转 - 返回解析是否成功*/
u8 ParseCmd_R(char * pCmdStr, CMD_ST * pstCmd)
{
    if(pCmdStr == NULL)
    {
        return 0;
    }

    fprintf(stderr,"\t%s:\t开始解析\n",pCmdStr);

    if (*pCmdStr != 'R' && *pCmdStr != 'r')
    {
        assert(0);
        return 0;
    }

    // r240
    errno = 0;
    int val = strtol(pCmdStr+1,NULL,10);
    if (errno != 0)
    {
        fprintf(stderr,"解析错误");
        return 0;
    }


    fprintf(stderr,"\t设置目标位置:%d\n",val);

    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stCond.m_ucCondtionType = COND_TYPE_ONCE;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_iActionType = ACTION_TYPE_POS;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_stActionPos.m_slPos = val;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_stActionPos.m_ucType = 0;
    pstCmd->m_ucLen++;

    fprintf(stderr,"\t解析完成,命令ID = %u\n",(unsigned)pstCmd->m_ucLen);

    return 1;
}

// speed set.
u8 ParseCmd_P(char * pCmdStr, CMD_ST *pstCmd)
{
    if(pCmdStr == NULL)
    {
        return 0;
    }

    fprintf(stderr,"\t%s:\t开始解析\n",pCmdStr);

    if (*pCmdStr != 'P' && *pCmdStr != 'p')
    {
        assert(0);
        return 0;
    }

    // r240
    errno = 0;
    int val = strtol(pCmdStr+1,NULL,10);
    if (errno != 0)
    {
        fprintf(stderr,"解析错误");
        return 0;
    }

    fprintf(stderr,"\t设置目标速度:`%d\n",val);

    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stCond.m_ucCondtionType = COND_TYPE_ONCE;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_iActionType = ACTION_TYPE_SPD;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_stActionSpeed.m_slSpeed = val;
    pstCmd->m_ucLen++;

    fprintf(stderr,"\t解析完成,命令ID = %u\n",(unsigned)pstCmd->m_ucLen);

    return 1;
}
// 设置IO. / shM7.
// 信号名称 - shM7.
// 格式: setio = sh + pos + sigName
//             = sl + pos + sigName.
u8 ParseCmd_S(char * pCmdStr,  CMD_ST *pstCmd)
{
    static s8 s_ascSigBuf[40];
    static s8 s_ascPosBuf[40];

    u8 ucSigLevel = 0xFF;
    char * pEnd;
    char * pStart;
    char scBackup;
    CONDITION_POS_ST stPos;
    CONDITION_SIG_ST stSig;

    if(pCmdStr == NULL)
    {
        return 0;
    }

    fprintf(stderr,"\t%s:\t开始解析\n",pCmdStr);

    if (*pCmdStr != 'S' && *pCmdStr != 's')
    {
        assert(0);
        return 0;
    }

    pCmdStr++;

    if (*pCmdStr == 'h' || *pCmdStr == 'H')
    {
        ucSigLevel = 1;
    }
    else if (*pCmdStr == 'l' || *pCmdStr == 'L')
    {
        ucSigLevel = 0;
    }  
    else
    {
        return 0; // command not recognized.
    }

    errno = 0;

    pCmdStr++;
    
    // 确认是否是位置,定位信号名.
    if (*pCmdStr == POS_PREFIX) // 使用的是指定位置的方式
    {
        pStart = pCmdStr + 1;
    }
    else
    {
        pStart = pCmdStr;
    }

    // 查找有效信号名称.
    if ((pEnd = strchr(pStart,'H')) == NULL
            &&(pEnd = strchr(pStart,'M')) == NULL) 
    {
        ERRINFO("未指定有效的信号名");
        return 0;
    }

    // 确认信号.

    if (HfnGetSig(pEnd,&stSig) == false)
    {
        ERRINFO("未指定有效的信号名");
    }
    else
    {
        strcpy((char*)s_ascSigBuf,pEnd);
        s_ascSigBuf[strlen(pEnd)] = '\0';
    }

    // 确定位置条件.
    scBackup = *pEnd;
    *pEnd = '\0';
    if (HfnGetPos(pCmdStr,&stPos) == false)
    {
        ERRINFO("未指定有效的信号名");
    }
    else
    {
        strcpy((char*)s_ascPosBuf,pCmdStr);
        s_ascPosBuf[strlen(pCmdStr)] = '\0';
    }
    *pEnd = scBackup;

    // sh/sl.
    fprintf(stderr,"\t设置信号:%s 为 %d,位置:%s\n",
            s_ascSigBuf,ucSigLevel,s_ascPosBuf);

#if 0
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stCond.m_ucCondtionType = COND_TYPE_ONCE;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_iActionType = ACTION_TYPE_SPD;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_stActionSpeed.m_slSpeed = val;
#endif
    pstCmd->m_ucLen++;

    fprintf(stderr,"\t解析完成,命令ID = %u\n",(unsigned)pstCmd->m_ucLen);

    return 1;
}

// 查询信号.
// 根据查询条件执行动作.
// 格式:
// condAction = il + pos + sig + action.
//            = ih + pos + sig + action.
//            ih 230-20MA-p5
//            ih 230-20 MA - p5
//            op pos   sig - p5
u8 ParseCmd_I(char * pCmdStr, CMD_ST *pstCmd)
{
    static s8 s_ascSigBuf[40];
    static s8 s_ascPosBuf[40];
    static s8 s_ascActionBuf[40];

    u8 ucSigLevel = 0xFF;
    char * pEnd;
    char * pStart;
    char scBackup;
    CONDITION_POS_ST stPos;
    CONDITION_SIG_ST stSig;
    ACTION_ST   stAction;

    if(pCmdStr == NULL)
    {
        return 0;
    }

    fprintf(stderr,"\t%s:\t开始解析\n",pCmdStr);

    if (*pCmdStr != 'I' && *pCmdStr != 'i')
    {
        assert(0);
        return 0;
    }

    pCmdStr++;

    if (*pCmdStr == 'h' || *pCmdStr == 'H')
    {
        ucSigLevel = 1;
    }
    else if (*pCmdStr == 'l' || *pCmdStr == 'L')
    {
        ucSigLevel = 0;
    }  
    else
    {
        return 0; // command not recognized.
    }

    errno = 0;
    pCmdStr++;
    
    // 确认是否是位置,定位信号名.
    if (*pCmdStr == POS_PREFIX) // 使用的是指定位置的方式
    {
        pStart = pCmdStr + 1;
    }
    else
    {
        pStart = pCmdStr;
    }

    // 查找有效信号名称.
    if ((pEnd = strchr(pStart,'H')) == NULL
            &&(pEnd = strchr(pStart,'M')) == NULL) 
    {
        ERRINFO("未指定有效的信号名");
        return 0;
    }

    // 位置信号.
    scBackup = *pEnd;
    *pEnd = '\0';
    strcpy((char*)s_ascPosBuf,pCmdStr);
    *pEnd = scBackup;
    if (HfnGetPos((char*)s_ascPosBuf,&stPos) == false)
    {
        ERRINFO("未指定有效的信号名");
        return 0;
    }
    // 确认信号. 信号和动作之间的分隔符是'-'
    pStart = pEnd;
    if ((pEnd = strchr(pStart,'-')) == NULL)
    {
        ERRINFO("未指定有效动作");
        return 0;
    }

    scBackup = *pEnd;
    *pEnd = '\0';
    strcpy((char*)s_ascSigBuf,pStart);
    *pEnd = scBackup;

    if (HfnGetSig(pStart,&stSig) == false)
    {
        ERRINFO("未指定有效的信号名");
        return 0;
    }

    pStart = pEnd + 1; // 跳过分隔符.
    strcpy((char*)s_ascActionBuf,pStart);

    if (HfnGetAction((char*)s_ascActionBuf,&stAction) == false)
    {
        ERRINFO("动作字符串无法解析:%s",pStart);
        return 0;
    }

    // sh/sl.
    fprintf(stderr,"\t查询信号:%s ,位置:%s，若信号值为%d，执行动作%s\n",
            s_ascSigBuf,s_ascPosBuf,ucSigLevel,s_ascActionBuf);

#if 0
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stCond.m_ucCondtionType = COND_TYPE_ONCE;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_iActionType = ACTION_TYPE_SPD;
    pstCmd->m_astNodeChain[pstCmd->m_ucLen].m_stAction.m_stActionSpeed.m_slSpeed = val;
#endif
    pstCmd->m_ucLen++;

    fprintf(stderr,"\t解析完成,命令ID = %u\n",(unsigned)pstCmd->m_ucLen);

    return 1;
}

void ParseCmd(char * pCmdStr,CMD_ST *pstCmd)
{
    switch(*pCmdStr)
    {
        case 'R':
        case 'r':
            ParseCmd_R(pCmdStr,pstCmd);
            break;
        case 'S':
        case 's':
            ParseCmd_S(pCmdStr,pstCmd);
            break;
        case 'P':
        case 'p':
            ParseCmd_P(pCmdStr,pstCmd);
            break;
        case 'I':
        case 'i':
            ParseCmd_I(pCmdStr,pstCmd);
            break;
        default:
            fprintf(stderr,"undefined options/command\n");
            break;
    }
}

void SplitCmd(char * pCmd)
{
    char * pCur;

    if (pCmd == NULL)
    {
        return;
    }

    while(*pCmd != '\0')
    {
        pCur = strchr(pCmd,CMD_SEPARATOR);

        if (pCur != NULL)
        {
            *pCur = '\0';// end current command.
        }

        ParseCmd(pCmd,&g_stCmd);

        if (pCur == NULL)
        {
            break;
        }

        pCmd = pCur+1;
    }
}
