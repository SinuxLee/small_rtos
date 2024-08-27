#define  IN_OS_CPU_C
#include "config.h"

extern idata uint8 STACK[1];                    /* ��ջ��ʼλ��,��OS_CPU_A���� */
uint8 idata * data OSTsakStackBotton[OS_MAX_TASKS + 2];/* �����ջ�ײ�λ��            */

#if EN_SP2 > 0
uint8 idata  Sp2[Sp2Space];                     /* �߼��ж϶�ջ           */
#endif

#if OS_MAX_TASKS < 9            
uint8 data OSFastSwap = 0xff;                   /* �����Ƿ���Կ����л� */
#else
uint16 data OSFastSwap = 0xffff;
#endif

/*********************************************************************************************************
** ��������: OSIdle
** ��������: ���ȼ���͵�����
** �䡡��: ��
** �䡡�� : ��
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

void OSIdle(void)
{
    while(1)
    {
        PCON = PCON | 0x01;                     /* CPU��������״̬ */
    }
}

/*********************************************************************************************************
** ��������: OSStart
** ��������: Small RTOS 51��ʼ������,���ô˺����������ʼ����,����ִIDΪ0������
** �䡡��: ��
** �䡡�� : ��
** ȫ�ֱ���: OSTsakStackBotton,SP
** ����ģ��: ��
********************************************************************************************************/
void OSStart(void)        
{
    uint8 idata *cp;
    uint8 i;
    
    cp = STACK;
    
    OSTsakStackBotton[0] = STACK;
    OSTsakStackBotton[OS_MAX_TASKS + 1] = (uint8 idata *)(IDATA_RAM_SIZE % 256);
    
    /* ��ʼ�����ȼ���ߵ������ջ��ʹ���ص�ַΪ����ʼ��ַ */
    *cp++ = ((uint16)(TaskFuction[0])) % 256;
    SP = (uint8) cp;
    *cp = ((uint16)(TaskFuction[0])) / 256;

    /* ��ʼ�����ȼ���͵������ջ */
    cp = (uint8 idata *)(IDATA_RAM_SIZE - 1) ;
    *cp-- = 0;
    *cp-- =  ((uint16)(OSIdle)) / 256;
    OSTsakStackBotton[OS_MAX_TASKS] = cp;
    *cp-- =  ((uint16)(OSIdle)) % 256;
    
    /* ��ʼ���������ȼ��������ջ */
    for(i = OS_MAX_TASKS - 1; i > 0; i--)
    {
        *cp-- = 0;
        *cp-- =  ((uint16)(TaskFuction[i])) / 256;
        OSTsakStackBotton[i] = cp;
        *cp-- =  ((uint16)(TaskFuction[i])) % 256;
    }
    /* �����ж� */
    Os_Enter_Sum = 1;
    OS_EXIT_CRITICAL();
    /* �����������ȼ���ߵ����� */
}

/*********************************************************************************************************
** ��������: OSTickISR
** ��������: ϵͳʱ���жϷ�����
** �䡡��: ��
** �䡡�� : ��
** ȫ�ֱ���: ��
** ����ģ��: OS_IBT_ENTER,(UserTickTimer),OSTimeTick,OSIntExit
********************************************************************************************************/
#if EN_OS_INT_ENTER >0 
#pragma disable                                        /* ����������ȼ��жϣ����򣬱��������һ��                 */
#endif
void OSTickISR(void) interrupt OS_TIME_ISR
{
#if TICK_TIMER_SHARING >1 
    static uint8 TickSum = 0;
#endif

#if EN_USER_TICK_TIMER > 0 
    UserTickTimer();                                    /* �û�����                                                 */
#endif

#if TICK_TIMER_SHARING >1 
    TickSum = (TickSum + 1) % TICK_TIMER_SHARING;
    if (TickSum != 0)
    {
        return;
    }
#endif

#if EN_OS_INT_ENTER > 0 
    OS_INT_ENTER();                                     /* �жϿ�ʼ����                                    */
#endif
    

#if EN_TIMER_SHARING > 0
    OSTimeTick();                                       /* ����ϵͳʱ�Ӵ�����                            */
#else
    OSIntSendSignal(TIME_ISR_TASK_ID);                  /* ����IDΪTIME_ISR_TASK_ID������                 */
#endif
    
    OSIntExit();                                        /* �жϽ�������                                             */
}
