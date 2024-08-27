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
** ��������: OSIntSendSignal
** ��������: �ж��и�ָ���������ź�,��ʹָ���������
** �䡡��: TaskId : ����ID
** �䡡��: ��
** ȫ�ֱ���: OSTaskRuning
** ����ģ��: ��
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
** ��������: OSSendSignal
** ��������: �����и�ָ���������ź�,��ʹָ���������
** �䡡��: TaskId : ����ID
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OSSched,OSIntSendSignal
********************************************************************************************************/
void OSSendSignal(uint8 TaskId)
{
    OSIntSendSignal(TaskId);
    OSSched();                                              //��ʼ�����л�
}

/*********************************************************************************************************
** ��������: OSClearSignal
** ��������: ���ָ�������ź�,��ʹָ����������
** �䡡��: TaskId : ����ID
** �䡡��: ��
** ȫ�ֱ���: OSTaskRuning
** ����ģ��: ��
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
** ��������: OSIntExit
** ��������: �ж��˳���������,�ڴ˽����жϺ�������л�
** �䡡��: ��
** �䡡��: ��
** ȫ�ֱ���: OSIntNesting,OSNextTaskID
** ����ģ��: OSIntCtxSw
********************************************************************************************************/
void OSIntExit(void)
{
    uint8 temp;

    OS_ENTER_CRITICAL();
                /* �ж�Ƕ�״��� */
#if EN_OS_INT_ENTER > 0
    if (OSIntNesting > 0)
    {
        OSIntNesting--;
    }
    if (OSIntNesting == 0)
    {
#endif
        
        Os_Enter_Sum = 0;               /* ��Ϊ���ж��У����Թ��жϼ�����Ϊ0 */
#if OS_MAX_TASKS < 9
                /* ���Ҵ��ھ���״̬�����������ȼ���ߵ����� */
        temp = OSTaskRuning;
        for (OSNextTaskID = 0; OSNextTaskID < OS_MAX_TASKS; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
        OSIntCtxSw();                   /* ����������� */
#else
                /* ���Ҵ��ھ���״̬�����������ȼ���ߵ����� */
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
        OSIntCtxSw();                   /* ����������� */
#endif

#if EN_OS_INT_ENTER >0
    }
#endif
    OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** ��������: OSSched
** ��������: ���жϵ������л�����
** �䡡��: ��
** �䡡��: ��
** ȫ�ֱ���: OSIntNesting,OSNextTaskID
** ����ģ��: OS_TASK_SW
********************************************************************************************************/
void  OSSched(void)
{
    uint8 temp;

    OS_ENTER_CRITICAL();
#if EN_OS_INT_ENTER > 0
    if (OSIntNesting == 0)              /* �Ƿ����ж��е��� */
    {
#endif

#if OS_MAX_TASKS < 9
                /* ���Ҵ��ھ���״̬�����������ȼ���ߵ����� */
        temp = OSTaskRuning;
        for (OSNextTaskID = 0; OSNextTaskID < OS_MAX_TASKS; OSNextTaskID++)
        {
            if ((temp & 0x01) != 0)
            {
                break;
            }
            temp = temp >> 1;
        }
        OS_TASK_SW();                   /* ����������� */
#else
                /* ���Ҵ��ھ���״̬�����������ȼ���ߵ����� */
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
        OS_TASK_SW();                   /* ����������� */
#endif

#if EN_OS_INT_ENTER > 0
    }
#endif
    OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** ��������: OSTimeTick
** ��������: ϵͳʱ�Ӵ�������,���������������ʱ
** �䡡��: ��
** �䡡��: ��
** ȫ�ֱ���: OSWaitTick
** ����ģ��: OSIntSendSignal
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
** ��������: OSWait
** ��������: ϵͳ�ȴ�����,������ô˺������Եȴ�һ��ʱ����ź�
** �䡡��: typ: �ȴ��¼�����,Ŀǰ����ȡ����ֵ,�������������ֵ�İ�λ��
**             K_SIG: �ȴ��ź�
**             K_TMO: �ȴ���ʱ
**        ticks : �ȴ���ʱʱ��ϵͳ�����
** �䡡�� : NOT_OK : ��������
**         TMO_EVENT : ��ʱ��
**         SIG_EVENT : ���ź�
** ȫ�ֱ���: OSWaitTick
** ����ģ��: OSClearSignal
********************************************************************************************************/
uint8 OSWait(uint8 typ, uint8 ticks)
{
    OSWaitTick[OSTaskID] = ticks;               /* ���ó�ʱʱ��         */
                                                /* �����Ż��Ĵ�����ʹ��  */
    switch(typ)
    {
    case K_SIG:                                 /* �ȴ��źţ��������Լ�  */
        OSWaitTick[OSTaskID] = 0;               /* û�г�ʱ����         */
        OSClearSignal(OSTaskID);                /* �������ȴ�״̬     */
        OSSched();                              /* ������һ������       */
        return SIG_EVENT;
    case K_TMO:                                 /* �ȴ���ʱ������ʱһ��ʱ�� */
        OS_ENTER_CRITICAL();
        while (OSWaitTick[OSTaskID] != 0)       /* �жϳ�ʱʱ���Ƿ�   */
        {
            OSClearSignal(OSTaskID);            /* �������ȴ�״̬     */
            OSSched();                          /* ������һ������       */
        }
        OS_EXIT_CRITICAL();
        return TMO_EVENT;
    case (K_TMO | K_SIG):                       /* �ȴ��źţ������Լ���ֱ����ʱ  */
                                                /* ���������жϿ��Իָ��� */
        OS_ENTER_CRITICAL();
        if (OSWaitTick[OSTaskID] == 0)          /* �жϳ�ʱʱ���Ƿ�   */
        {
            return TMO_EVENT;
        }
        OSClearSignal(OSTaskID);                /* �������ȴ�״̬     */
        OS_EXIT_CRITICAL();
        OSSched();                              /* ������һ������       */
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