#define  IN_OS_CPU_C
#include "config.h"

extern idata uint8 STACK[1];                    /* 堆栈起始位置,在OS_CPU_A定义 */
uint8 idata * data OSTsakStackBotton[OS_MAX_TASKS + 2];/* 任务堆栈底部位置            */

#if EN_SP2 > 0
uint8 idata  Sp2[Sp2Space];                     /* 高级中断堆栈           */
#endif

#if OS_MAX_TASKS < 9            
uint8 data OSFastSwap = 0xff;                   /* 任务是否可以快速切换 */
#else
uint16 data OSFastSwap = 0xffff;
#endif

/*********************************************************************************************************
** 函数名称: OSIdle
** 功能描述: 优先级最低的任务
** 输　入: 无
** 输　出 : 无
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/

void OSIdle(void)
{
    while(1)
    {
        PCON = PCON | 0x01;                     /* CPU进入休眠状态 */
    }
}

/*********************************************************************************************************
** 函数名称: OSStart
** 功能描述: Small RTOS 51初始化函数,调用此函数后多任务开始运行,首先执ID为0的任务
** 输　入: 无
** 输　出 : 无
** 全局变量: OSTsakStackBotton,SP
** 调用模块: 无
********************************************************************************************************/
void OSStart(void)        
{
    uint8 idata *cp;
    uint8 i;
    
    cp = STACK;
    
    OSTsakStackBotton[0] = STACK;
    OSTsakStackBotton[OS_MAX_TASKS + 1] = (uint8 idata *)(IDATA_RAM_SIZE % 256);
    
    /* 初始化优先级最高的任务堆栈，使返回地址为任务开始地址 */
    *cp++ = ((uint16)(TaskFuction[0])) % 256;
    SP = (uint8) cp;
    *cp = ((uint16)(TaskFuction[0])) / 256;

    /* 初始化优先级最低的任务堆栈 */
    cp = (uint8 idata *)(IDATA_RAM_SIZE - 1) ;
    *cp-- = 0;
    *cp-- =  ((uint16)(OSIdle)) / 256;
    OSTsakStackBotton[OS_MAX_TASKS] = cp;
    *cp-- =  ((uint16)(OSIdle)) % 256;
    
    /* 初始化其它优先级的任务堆栈 */
    for(i = OS_MAX_TASKS - 1; i > 0; i--)
    {
        *cp-- = 0;
        *cp-- =  ((uint16)(TaskFuction[i])) / 256;
        OSTsakStackBotton[i] = cp;
        *cp-- =  ((uint16)(TaskFuction[i])) % 256;
    }
    /* 允许中断 */
    Os_Enter_Sum = 1;
    OS_EXIT_CRITICAL();
    /* 函数返回优先级最高的任务 */
}

/*********************************************************************************************************
** 函数名称: OSTickISR
** 功能描述: 系统时钟中断服务函数
** 输　入: 无
** 输　出 : 无
** 全局变量: 无
** 调用模块: OS_IBT_ENTER,(UserTickTimer),OSTimeTick,OSIntExit
********************************************************************************************************/
#if EN_OS_INT_ENTER >0 
#pragma disable                                        /* 除非最高优先级中断，否则，必须加上这一句                 */
#endif
void OSTickISR(void) interrupt OS_TIME_ISR
{
#if TICK_TIMER_SHARING >1 
    static uint8 TickSum = 0;
#endif

#if EN_USER_TICK_TIMER > 0 
    UserTickTimer();                                    /* 用户函数                                                 */
#endif

#if TICK_TIMER_SHARING >1 
    TickSum = (TickSum + 1) % TICK_TIMER_SHARING;
    if (TickSum != 0)
    {
        return;
    }
#endif

#if EN_OS_INT_ENTER > 0 
    OS_INT_ENTER();                                     /* 中断开始处理                                    */
#endif
    

#if EN_TIMER_SHARING > 0
    OSTimeTick();                                       /* 调用系统时钟处理函数                            */
#else
    OSIntSendSignal(TIME_ISR_TASK_ID);                  /* 唤醒ID为TIME_ISR_TASK_ID的任务                 */
#endif
    
    OSIntExit();                                        /* 中断结束处理                                             */
}
