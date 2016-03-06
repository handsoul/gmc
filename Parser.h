#ifndef __PARSER_H__
#define __PARSER_H__

// reference type definition.
#include "DataType.h"

// command seperator char
#define CMD_SEPARATOR           '/'
#define END_CHAR                '\0'

#define SIG_PREFIX          'M'
#define POS_PREFIX          'M'

#define CMD_INTERNAL_SEPERATOR  '-'

#define CMD_BUFSIZE             20
#define CMD_CHAIN_MAX_LEN       20
#define SYS_RESOURCE_MAX_LEN    20

// extra options.
#define DEBUG_CMD

#ifdef DEBUG_CMD
#define ERRINFO(format...) \
    fprintf(stderr,format)
#else
    #define ERRINFO(format...)
#endif

//#define VERBOSE_PRINT

#ifdef VERBOSE_PRINT
#define TRACE_PRINT ERRINFO
#else
#define TRACE_PRINT(format...)
#endif

// 资源类型: 数字输入.
typedef struct tagResourceItemDI
{
    u8  m_ucIndex;
    s32 m_slvalue;
}R_DI;

// 资源类型: 数字输出.
typedef struct tagResourceItemDO
{
    u8  m_ucIndex;
    s32 m_slvalue;
}R_DO;

// 资源类型: 模拟输入.
typedef  struct tagResourceAI
{
    u8  m_ucIndex;
    s32 m_slvalue;
}R_AI;

// 资源类型: 模拟输出.
typedef  struct tagResourceAO
{
    u8  m_ucIndex;
    s32 m_slvalue;
}R_AO;

// 资源类型定义.
typedef struct tagRItem
{
    u8 m_ucRType;
    union
    {
        R_DI m_unR_DI;
        R_DO m_unR_DO;
        R_AI m_unR_AI;
        R_AO m_unR_AO;
    };
}RESOURCE_ITEM;


// 运行命令 - 类型1 - 绝对运行命令.
typedef struct tagCmd_R1
{
    s32 m_slPos;
}CMD_R1_ST;

// 运行命令 - 类型2 - 相对运行命令.
typedef struct tagCmd_R2
{
    s32 m_slPos;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_R2_ST;

// 运行命令  - 类型3 - 运行到寄存器位置.
typedef struct tagCMD_R3
{
    u8 m_ucRegNo;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_R3_ST;

// 运行命令 - 类型4 - 运行到原点.
typedef struct tagCMD_R4
{
    u8 m_ucReserved;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_R4_ST;

// 速度设置指令 - 设置一定的运行速度
typedef struct tagCMD_P
{
    u8 m_ucSpeedIndex;
    s32 m_slSpeed;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_P_ST;

// 信号设置命令 - 设置信号为低
typedef struct tagCMD_SL
{
    u8 m_ucSigIndex;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_SL_ST;

// 信号设置命令 - 设置信号为高
typedef struct tagCMD_SH
{
    u8 m_ucSigIndex;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_SH_ST;

// 信号查询命令(条件命令). 查询信号为低则执行
typedef struct tagCMD_IL
{
    u8 m_ucSigIndex;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_IL_ST;

// 信号查询命令(条件命令). 查询信号为高则执行
typedef struct tagCMCD_IH
{
    u8 m_ucSigIndex;
#ifdef DEBUG_CMD
    s8  m_ascStr[CMD_BUFSIZE];
#endif
}CMD_IH_ST;

// 思路: command.

// 执行链.
// 执行节点: 条件 + 动作

typedef struct tagConditonSig
{
    u8 m_iRid;
    s32 m_iTriggleValue;
}CONDITION_SIG_ST;

typedef struct tagConditonPos
{
    s32 m_slPos;
    s32 m_slOffset;
    u8  m_ucRelation;
}CONDITION_POS_ST;

typedef enum tagEnumCondType
{
    COND_TYPE_ONCE,
    COND_TYPE_SIG,
    COND_TYPE_POS,
}E_COND_TYPE;

typedef struct tagCondition
{
    u8 m_ucCondtionType;
    union
    {
        CONDITION_POS_ST m_stCondPos;
        CONDITION_SIG_ST m_stCondSig;
    };

}CONDITION_ST;

// 调整信号
typedef struct tagActionSig
{
    u8  m_ucRid;
    s32 m_slSetVal;
    // the member below is ugly.
    s32 m_slPos;
}ACTION_SIG_ST;

// 调整运动参数 - 目标速度.
typedef struct tagActionSpeed
{
    s32 m_slSpeed;  // 速度信号
}ACTION_SPEED_ST;

// 调整运动参数 - 终点位置
typedef struct tagActionPos
{
    s32 m_slPos;
    u8  m_ucType;// 绝对/增量方式
}ACTION_POS_ST;

typedef enum tagEnumActionType
{
    ACTION_TYPE_NONE,
    ACTION_TYPE_SIG,
    ACTION_TYPE_POS,
    ACTION_TYPE_SPD,
    
}E_ACTION_TYPE;

typedef struct tagAction
{
    int m_iActionType;
    union
    {
        ACTION_SIG_ST m_stActionSig;
        ACTION_POS_ST m_stActionPos;
        ACTION_SPEED_ST m_stActionSpeed;
    };
}ACTION_ST;


typedef struct tagENode
{
    CONDITION_ST m_stCond;
    ACTION_ST	 m_stAction;
#ifdef ENABLE_EXTENDED_FILED
    u8  m_ucPri;		// 优先级.
    u8  m_ucEnable;     // 使能.
    void (*m_pFnPrintExtraInfo)(struct tagENode *);
#endif
}ENODE_ST;

// 最终命令的形态.
typedef struct tagCMD
{
	// 执行链.
	ENODE_ST m_astNodeChain[CMD_CHAIN_MAX_LEN];
    u8 m_ucLen;
    u8 m_ucJobDone;
}CMD_ST;

// 运行状态
typedef struct tagRunState
{
    // 资源
    RESOURCE_ITEM m_astResource[SYS_RESOURCE_MAX_LEN];
    // 目标速度
    s32 m_slDstSpeed;
    // 目标位置
    s32 m_slDstPos;
}RUN_STATE_ST;

typedef struct tagExecuteUnit
{
    CMD_ST m_stCmd;
    RUN_STATE_ST m_stRunState; // 当前系统状态
}ST_EU;

void SplitCmd(char * pCmd);

#endif // __PARSER_H__
