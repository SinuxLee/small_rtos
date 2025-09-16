#define  IN_OS_SEM
#include "config.h"

/* 分配信号量存储空间 */
#if EN_OS_SEM > 0
#if OS_MAX_TASKS < 9
uint8 OS_SEM_MEM_SEL OsSemBuf[OS_MAX_SEMS * 2];
#else
uint8 OS_SEM_MEM_SEL OsSemBuf[OS_MAX_SEMS * 3];
#endif


/*********************************************************************************************************
** 函数名称: OSSemCreate
** 功能描述: 初始化消息队列
** 输　入: Index:信号量索引
**         data:信号量初始值
** 输　出: NOT_OK:没有这个信号量
**         OS_SEM_OK:成功
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/
uint8 OSSemCreate(uint8 Index,uint8 Data)
{
    OS_ENTER_CRITICAL();
    if (Index < OS_MAX_SEMS )
    {
#if OS_MAX_TASKS < 9
        OsSemBuf[2 * Index] = Data;             /* 计数器置初值 */
        OsSemBuf[2 * Index + 1] = 0;            /* 清空等待队列 */
#else
        OsSemBuf[3 * Index] = Data;             /* 计数器置初值 */
                /* 清空等待队列 */
        OsSemBuf[3 * Index + 1] = 0;
        OsSemBuf[3 * Index + 2] = 0;
#endif
        OS_EXIT_CRITICAL();
        return OS_SEM_OK;
    }
    OS_EXIT_CRITICAL();
    return NOT_OK;
}

/*********************************************************************************************************
** 函数名称: OSSemPend
** 功能描述: 等待一个信号量
** 输　入: Index:信号量索引
**         Tick:等待时间
** 输　出: NOT_OK:参数错误
**         OS_SEM_OK:得到信号量
**         OS_SEM_TMO:超时到
**         OS_SEM_NOT_OK:没有得到信号量
** 全局变量: 无
** 调用模块: OSRunningTaskID,OSClearSignal,OSSched,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_SEM_PENT > 0

uint8 OSSemPend(uint8 Index, uint8 Tick)
{
#if EN_OS_SEM_CHK > 0
    if (Index >= OS_MAX_SEMS)
    {
        return 0;
    }
#endif

    OS_ENTER_CRITICAL();

    OSWaitTick[OSRunningTaskID()] = Tick;       /* 设置超时时间         */
                                                /* 可以优化寄存器的使用  */
                /* 把任务加入等待任务队列 */
#if OS_MAX_TASKS < 9
    OsSemBuf[Index * 2 + 1] |= OSMapTbl[OSRunningTaskID()];
#else
    if (OSRunningTaskID() < 8)
    {
        OsSemBuf[Index * 3 + 1] |= OSMapTbl[OSRunningTaskID()];
    }
    else
    {
        OsSemBuf[Index * 3 + 2] |= OSMapTbl[OSRunningTaskID() & 0x07];
    }
#endif
                /* 信号量是否有效 */
#if OS_MAX_TASKS < 9
    while (OsSemBuf[Index * 2] == 0)
    {
#else
    while (OsSemBuf[Index * 3] == 0)
    {
#endif
                /* 使用堆栈是为了使函数具有重入性 */
#ifdef __C51__
        SP++;
        *((uint8 data *)SP) = Index;
#endif
            /* 信号量无效 */
        OSClearSignal(OSRunningTaskID());   /* 任务进入等待状态 */
        OSSched();                          /* 运行下一个任务 */

#ifdef __C51__
        Index = *((uint8 data *)SP);
        SP--;
#endif
             /* 任务再次运行，如果超时到，退出循环 */
        if (OSWaitTick[OSRunningTaskID()] == 0)
        {
            break;
        }
    }

            /* 将任务从等待队列中清除（可以删除） */
#if OS_MAX_TASKS < 9
    OsSemBuf[Index * 2 + 1] &= ~OSMapTbl[OSRunningTaskID()];
#else
    if (OSRunningTaskID() < 8)
    {
        OsSemBuf[Index * 3 + 1] &= ~OSMapTbl[OSRunningTaskID()];
    }
    else
    {
        OsSemBuf[Index * 3 + 2] &= ~OSMapTbl[OSRunningTaskID() & 0x07];
    }
#endif
            /* 判断信号量是否有效。有效，信号量计数器减一 */
#if OS_MAX_TASKS < 9
    if (OsSemBuf[Index * 2] > 0)
    {
        OsSemBuf[Index * 2]--;
#else
    if (OsSemBuf[Index * 3] > 0)
    {
        OsSemBuf[Index * 3]--;
#endif
        OS_EXIT_CRITICAL();
        return OS_SEM_OK;
    }
    else
    {
        /* 无信号返回信号无效 */
        OS_EXIT_CRITICAL();
        return OS_SEM_TMO;
    }
}
#endif

/*********************************************************************************************************
** 函数名称: OSSemAccept
** 功能描述: 无等待请求信号量
** 输　入: Index:信号量索引
** 输　出: NOT_OK:参数错误
**         OS_SEM_OK:得到信号量
**         OS_SEM_TMO:超时到
**         OS_SEM_NOT_OK:没有得到信号量
** 全局变量: 无
** 调用模块: OSClearSignal,OSSched,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_SEM_ACCEPT > 0
uint8 OSSemAccept(uint8 Index)
{
#if EN_OS_SEM_CHK > 0
    if (Index >= OS_MAX_SEMS)
    {
        return 0;
    }
#endif

    OS_ENTER_CRITICAL();
            /* 判断信号量是否有效。有效，信号量计数器减一 */
#if OS_MAX_TASKS < 9
    if (OsSemBuf[Index * 2] > 0)
    {
        OsSemBuf[Index * 2]--;
#else
    if (OsSemBuf[Index * 3] > 0)
    {
        OsSemBuf[Index * 3]--;
#endif
        OS_EXIT_CRITICAL();
        return OS_SEM_OK;
    }
    else
    {
        /* 无信号返回信号无效 */
        OS_EXIT_CRITICAL();
        return OS_SEM_NOT_OK;
    }
}
#endif


/*********************************************************************************************************
** 函数名称: OSSemIntPost
** 功能描述: 中断中发送一个信号量
** 输　入: Index:信号量索引
** 输　出: NOT_OK:参数错误
**         OS_SEM_OK:发送成功
** 全局变量: 无
** 调用模块: OSIntSendSignal,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_SEM_INT_POST > 0  || EN_OS_SEM_POST > 0
uint8 OSSemIntPost(uint8 Index)
{
    uint8 temp,i;

#if EN_OS_SEM_CHK > 0
    if (Index >= OS_MAX_SEMS)
    {
        return NOT_OK;
    }
#endif

    OS_ENTER_CRITICAL();

#if OS_MAX_TASKS < 9
        /* 信号量计数器加一 */
    if (OsSemBuf[Index * 2] <255)
    {
        OsSemBuf[Index * 2]++;
    }
        /* 察看信号量的等待任务队列 */
    temp = OsSemBuf[Index * 2 + 1];
    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        if ((temp & 0x01) != 0)
        {
            break;
        }
        temp = temp >> 1;
    }
        /* 有任务等待信号，使其中优先级最高的进入就绪状态，并将其从等待队列中清除 */
    if (i < OS_MAX_TASKS)
    {
        OsSemBuf[Index * 2 + 1] &= ~OSMapTbl[i];
        OSIntSendSignal(i);
    }
#else
        /* 信号量计数器加一 */
    if (OsSemBuf[Index * 3] <255)
    {
        OsSemBuf[Index * 3]++;
    }

    temp = OsSemBuf[Index * 3 + 1];
    for (i = 0; i < 8; i++)
    {
        if ((temp & 0x01) != 0)
        {
            break;
        }
        temp = temp >> 1;
    }
    if (i >= 8)
    {
        temp = OsSemBuf[Index * 3 + 2];
        for ( ; i < OS_MAX_TASKS; i++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
    }
        /* 有任务等待信号，使其中优先级最高的进入就绪状态，并将其从等待队列中清除 */
    if (i < OS_MAX_TASKS)
    {
        if (i < 8)
        {
            OsSemBuf[Index * 3 + 1] &= ~OSMapTbl[i];
        }
        else
        {
            OsSemBuf[Index * 3 + 2] &= ~OSMapTbl[i & 0x07];
        }
        OSIntSendSignal(i);
    }
#endif
    OS_EXIT_CRITICAL();
    return OS_SEM_OK;
}
#endif

/*********************************************************************************************************
** 函数名称: OSSemPost
** 功能描述: 发送一个信号量
** 输　入: Index:信号量索引
** 输　出: NOT_OK:参数错误
**         OS_SEM_OK:发送成功
** 全局变量: 无
** 调用模块: OSSemIntPost,OSSched
********************************************************************************************************/
#if EN_OS_SEM_POST > 0
uint8 OSSemPost(uint8 Index)
{
    if (OSSemIntPost(Index) == OS_SEM_OK)
    {
        OSSched();
        return OS_SEM_OK;
    }
    else
    {
        return NOT_OK;
    }
}
#endif

/*********************************************************************************************************
** 函数名称: OSSemQuery
** 功能描述: 查询信号量
** 输　入: Index:信号量索引
** 输　出: 信号量的值
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_SEM_QUERY > 0
uint8 OSSemQuery(uint8 Index)
{
    if (Index >= OS_MAX_SEMS)
    {
        return 0;
    }

#if OS_MAX_TASKS < 9
        return OsSemBuf[2*Index];
#else
        return OsSemBuf[3*Index];
#endif
}
#endif

#endif
