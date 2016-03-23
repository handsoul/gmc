#include "Execute.h"
#include "Parser.h"

static bool IsConditionSatisfy(const CONDITION_ST * pstCond, const RUN_STATE_ST * pstRunState)
{
    if (pstCond->m_ucCondtionType == COND_TYPE_SIG)  // 信号条件达到要求
    {
        // 查找在系统状态中
        return (pstRunState->m_astResource[pstCond->m_stCondSig.m_iRid].m_unR_DI.m_slvalue 
            == pstCond->m_stCondSig.m_iRid);
    }
    else if (pstCond->m_ucCondtionType == COND_TYPE_POS)
    {
        return (abs(pstRunState->m_slCurPos - pstCond->m_stCondPos.m_slPos) <= 1); 
    }

    return false;
}

static void DoAciton(ACTION_ST * pstAction, RUN_STATE_ST *pstRunState)
{
    if (pstAction->m_iActionType == ACTION_TYPE_POS)
    {
        pstRunState->m_slDstPos = pstAction->m_stActionPos.m_slPos;
    }
    else if (pstAction->m_iActionType == ACTION_TYPE_SPD)
    {
        pstRunState->m_slDstSpeed = pstAction->m_stActionSpeed.m_slSpeed;
    }
    else if (pstAction->m_iActionType == ACTION_TYPE_SIG)
    {
        // todo: 
        // operation on signal (valid for digital/analog output only) 
        //
    }

    return;
}

void ExecuteCommand(ENODE_ST * pstNode, RUN_STATE_ST * pstRunState)
{
    if (IsConditionSatisfy(&pstNode->m_stCond,pstRunState))
    {
        DoAciton(&pstNode->m_stAction,pstRunState);
    }
}
