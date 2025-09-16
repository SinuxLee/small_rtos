#define  IN_OS_CORE
#include "config.h"

#if EN_OS_INT_ENTER > 0
uint8 OSIntNesting;
#endif

uint8 data OSTaskID = 0;
uint8 data OSNextTaskID = 0;

uint8 OSWaitTick[OS_MAX_TASKS];
#if OS_MAX_TASKS < 9
uint8 OSTaskRuning = 0xff;
#else
uint16 OSTaskRuning = 0xffff;
#endif

uint8 const OSMapTbl[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00};
uint8 data Os_Enter_Sum = 0;


void  OSSched(void);

/*********************************************************************************************************
** 函数名称: OSIntSendSignal
** 功能描述: 中断中给指定任务发送信号,既使指定任务就绪
** 输　入: TaskId : 任务ID
** 输　出: 无
** 全局变量: OSTaskRuning
** 调用模块: 无
********************************************************************************************************/
void OSIntSendSignal(uint8 TaskId)
{
    if (TaskId < OS_MAX_TASKS)
    {
        OS_ENTER_CRITICAL();
#if OS_MAX_TASKS < 9
        OSTaskRuning |= OSMapTbl[TaskId];
#else
        if (TaskId < 8)
        {
            ((uint8 *)(&OSTaskRuning))[LOW_BYTE] |= OSMapTbl[TaskId];
        }
        else
        {
            ((uint8 *)(&OSTaskRuning))[HIGH_BYTE] |= OSMapTbl[TaskId & 0x07];
        }
#endif
        OS_EXIT_CRITICAL();
    }
}

/*********************************************************************************************************
** 函数名称: OSSendSignal
** 功能描述: 任务中给指定任务发送信号,既使指定任务就绪
** 输　入: TaskId : 任务ID
** 输　出: 无
** 全局变量: 无
** 调用模块: OSSched,OSIntSendSignal
********************************************************************************************************/
void OSSendSignal(uint8 TaskId)
{
    OSIntSendSignal(TaskId);
    OSSched();                                              //开始任务切换
}

/*********************************************************************************************************
** 函数名称: OSClearSignal
** 功能描述: 清除指定任务信号,既使指定任务休眠
** 输　入: TaskId : 任务ID
** 输　出: 无
** 全局变量: OSTaskRuning
** 调用模块: 无
********************************************************************************************************/
void OSClearSignal(uint8 TaskId)
{
    if (TaskId < OS_MAX_TASKS)
    {
        OS_ENTER_CRITICAL();
#if OS_MAX_TASKS < 9
        OSTaskRuning &= ~OSMapTbl[TaskId];
#else
        if (TaskId < 8)
        {
            ((uint8 *)(&OSTaskRuning))[LOW_BYTE] &= ~OSMapTbl[TaskId];
        }
        else
        {
            ((uint8 *)(&OSTaskRuning))[HIGH_BYTE] &= ~OSMapTbl[TaskId & 0x07];
        }
#endif
        OS_EXIT_CRITICAL();
    }
}

/*********************************************************************************************************
** 函数名称: OSIntExit
** 功能描述: 中断退出处理函数,在此进行中断后的任务切换
** 输　入: 无
** 输　出: 无
** 全局变量: OSIntNesting,OSNextTaskID
** 调用模块: OSIntCtxSw
********************************************************************************************************/
void OSIntExit(void)
{
    uint8 temp;

    OS_ENTER_CRITICAL();
                /* 中断嵌套处理 */
#if EN_OS_INT_ENTER > 0
    if (OSIntNesting > 0)
    {
        OSIntNesting--;
    }
    if (OSIntNesting == 0)
    {
#endif
        
        Os_Enter_Sum = 0;               /* 因为在中断中，所以关中断计数器为0 */
#if OS_MAX_TASKS < 9
                /* 查找处于就绪状态的任务中优先级最高的任务 */
        temp = OSTaskRuning;
        for (OSNextTaskID = 0; OSNextTaskID < OS_MAX_TASKS; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
        OSIntCtxSw();                   /* 进行任务调度 */
#else
                /* 查找处于就绪状态的任务中优先级最高的任务 */
        temp = OSTaskRuning % 256;
        for (OSNextTaskID = 0; OSNextTaskID < 8; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                goto TaskSw;
            }
            temp = temp >> 1;
        }

        temp = OSTaskRuning / 256;
        for (; OSNextTaskID < OS_MAX_TASKS; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
TaskSw:
        OSIntCtxSw();                   /* 进行任务调度 */
#endif

#if EN_OS_INT_ENTER >0
    }
#endif
    OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称: OSSched
** 功能描述: 非中断的任务切换函数
** 输　入: 无
** 输　出: 无
** 全局变量: OSIntNesting,OSNextTaskID
** 调用模块: OS_TASK_SW
********************************************************************************************************/
void  OSSched(void)
{
    uint8 temp;

    OS_ENTER_CRITICAL();
#if EN_OS_INT_ENTER > 0
    if (OSIntNesting == 0)              /* 是否是中断中调用 */
    {
#endif

#if OS_MAX_TASKS < 9
                /* 查找处于就绪状态的任务中优先级最高的任务 */
        temp = OSTaskRuning;
        for (OSNextTaskID = 0; OSNextTaskID < OS_MAX_TASKS; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
        OS_TASK_SW();                   /* 进行任务调度 */
#else
                /* 查找处于就绪状态的任务中优先级最高的任务 */
        temp = OSTaskRuning % 256;
        for (OSNextTaskID = 0; OSNextTaskID < 8; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                goto TaskSw;
            }
            temp = temp >> 1;
        }

        temp = OSTaskRuning / 256 ;
        for (; OSNextTaskID < OS_MAX_TASKS; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
TaskSw:
        OS_TASK_SW();                   /* 进行任务调度 */
#endif

#if EN_OS_INT_ENTER > 0
    }
#endif
    OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称: OSTimeTick
** 功能描述: 系统时钟处理函数,处理各个任务的延时
** 输　入: 无
** 输　出: 无
** 全局变量: OSWaitTick
** 调用模块: OSIntSendSignal
********************************************************************************************************/
void  OSTimeTick(void)
{
    uint8 i;

    for (i = 0; i < OS_MAX_TASKS; i++)                 
    {
        if (OSWaitTick[i] != 0 )
        {
            OSWaitTick[i]--;
            if (OSWaitTick[i] == 0)
            {
                OSIntSendSignal(i);
            }
        }
    }
}

/*********************************************************************************************************
** 函数名称: OSWait
** 功能描述: 系统等待函数,任务调用此函数可以等待一定时间或信号
** 输　入: typ: 等待事件类型,目前可以取以下值,或是其中任意个值的按位或
**             K_SIG: 等待信号
**             K_TMO: 等待超时
**        ticks : 等待超时时的系统嘀嗒数
** 输　出 : NOT_OK : 参数错误
**         TMO_EVENT : 超时到
**         SIG_EVENT : 有信号
** 全局变量: OSWaitTick
** 调用模块: OSClearSignal
********************************************************************************************************/
uint8 OSWait(uint8 typ, uint8 ticks)
{
    OSWaitTick[OSTaskID] = ticks;               /* 设置超时时间         */
                                                /* 可以优化寄存器的使用  */
    switch(typ)
    {
    case K_SIG:                                 /* 等待信号，即挂起自己  */
        OSWaitTick[OSTaskID] = 0;               /* 没有超时处理         */
        OSClearSignal(OSTaskID);                /* 任务进入等待状态     */
        OSSched();                              /* 运行下一个任务       */
        return SIG_EVENT;
    case K_TMO:                                 /* 等待超时，即延时一段时间 */
        OS_ENTER_CRITICAL();
        while (OSWaitTick[OSTaskID] != 0)       /* 判断超时时间是否到   */
        {
            OSClearSignal(OSTaskID);            /* 任务进入等待状态     */
            OSSched();                          /* 运行下一个任务       */
        }
        OS_EXIT_CRITICAL();
        return TMO_EVENT;
    case (K_TMO | K_SIG):                       /* 等待信号（挂起自己）直到超时  */
                                                /* 别的任务或中断可以恢复它 */
        OS_ENTER_CRITICAL();
        if (OSWaitTick[OSTaskID] == 0)          /* 判断超时时间是否到   */
        {
            return TMO_EVENT;
        }
        OSClearSignal(OSTaskID);                /* 任务进入等待状态     */
        OS_EXIT_CRITICAL();
        OSSched();                              /* 运行下一个任务       */
        if (OSWaitTick[OSTaskID] != 0)
        {
            OSWaitTick[OSTaskID] = 0;
            return SIG_EVENT;
        }
        return TMO_EVENT;
    default:
        OSWaitTick[OSTaskID] = 0;
        return NOT_OK;
    }
}
