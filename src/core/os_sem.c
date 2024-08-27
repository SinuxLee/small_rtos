#define  IN_OS_SEM
#include "config.h"

/* �����ź����洢�ռ� */
#if EN_OS_SEM > 0
#if OS_MAX_TASKS < 9
uint8 OS_SEM_MEM_SEL OsSemBuf[OS_MAX_SEMS * 2];
#else
uint8 OS_SEM_MEM_SEL OsSemBuf[OS_MAX_SEMS * 3];
#endif


/*********************************************************************************************************
** ��������: OSSemCreate
** ��������: ��ʼ����Ϣ����
** �䡡��: Index:�ź�������
**         data:�ź�����ʼֵ
** �䡡��: NOT_OK:û������ź���
**         OS_SEM_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
uint8 OSSemCreate(uint8 Index,uint8 Data)
{
    OS_ENTER_CRITICAL();
    if (Index < OS_MAX_SEMS )
    {
#if OS_MAX_TASKS < 9
        OsSemBuf[2 * Index] = Data;             /* �������ó�ֵ */
        OsSemBuf[2 * Index + 1] = 0;            /* ��յȴ����� */
#else
        OsSemBuf[3 * Index] = Data;             /* �������ó�ֵ */
                /* ��յȴ����� */
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
** ��������: OSSemPend
** ��������: �ȴ�һ���ź���
** �䡡��: Index:�ź�������
**         Tick:�ȴ�ʱ��
** �䡡��: NOT_OK:��������
**         OS_SEM_OK:�õ��ź���
**         OS_SEM_TMO:��ʱ��
**         OS_SEM_NOT_OK:û�еõ��ź���
** ȫ�ֱ���: ��
** ����ģ��: OSRunningTaskID,OSClearSignal,OSSched,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
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

    OSWaitTick[OSRunningTaskID()] = Tick;       /* ���ó�ʱʱ��         */
                                                /* �����Ż��Ĵ�����ʹ��  */
                /* ���������ȴ�������� */
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
                /* �ź����Ƿ���Ч */
#if OS_MAX_TASKS < 9
    while (OsSemBuf[Index * 2] == 0)
    {
#else
    while (OsSemBuf[Index * 3] == 0)
    {
#endif
                /* ʹ�ö�ջ��Ϊ��ʹ�������������� */
#ifdef __C51__
        SP++;
        *((uint8 data *)SP) = Index;
#endif
            /* �ź�����Ч */
        OSClearSignal(OSRunningTaskID());   /* �������ȴ�״̬ */
        OSSched();                          /* ������һ������ */

#ifdef __C51__
        Index = *((uint8 data *)SP);
        SP--;
#endif
             /* �����ٴ����У������ʱ�����˳�ѭ�� */
        if (OSWaitTick[OSRunningTaskID()] == 0)
        {
            break;
        }
    }

            /* ������ӵȴ����������������ɾ���� */
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
            /* �ж��ź����Ƿ���Ч����Ч���ź�����������һ */
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
        /* ���źŷ����ź���Ч */
        OS_EXIT_CRITICAL();
        return OS_SEM_TMO;
    }
}
#endif

/*********************************************************************************************************
** ��������: OSSemAccept
** ��������: �޵ȴ������ź���
** �䡡��: Index:�ź�������
** �䡡��: NOT_OK:��������
**         OS_SEM_OK:�õ��ź���
**         OS_SEM_TMO:��ʱ��
**         OS_SEM_NOT_OK:û�еõ��ź���
** ȫ�ֱ���: ��
** ����ģ��: OSClearSignal,OSSched,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
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
            /* �ж��ź����Ƿ���Ч����Ч���ź�����������һ */
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
        /* ���źŷ����ź���Ч */
        OS_EXIT_CRITICAL();
        return OS_SEM_NOT_OK;
    }
}
#endif


/*********************************************************************************************************
** ��������: OSSemIntPost
** ��������: �ж��з���һ���ź���
** �䡡��: Index:�ź�������
** �䡡��: NOT_OK:��������
**         OS_SEM_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OSIntSendSignal,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
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
        /* �ź�����������һ */
    if (OsSemBuf[Index * 2] <255)
    {
        OsSemBuf[Index * 2]++;
    }
        /* �쿴�ź����ĵȴ�������� */
    temp = OsSemBuf[Index * 2 + 1];
    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        if ((temp & 0x01) != 0)
        {
            break;
        }
        temp = temp >> 1;
    }
        /* ������ȴ��źţ�ʹ�������ȼ���ߵĽ������״̬��������ӵȴ���������� */
    if (i < OS_MAX_TASKS)
    {
        OsSemBuf[Index * 2 + 1] &= ~OSMapTbl[i];
        OSIntSendSignal(i);
    }
#else
        /* �ź�����������һ */
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
        /* ������ȴ��źţ�ʹ�������ȼ���ߵĽ������״̬��������ӵȴ���������� */
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
** ��������: OSSemPost
** ��������: ����һ���ź���
** �䡡��: Index:�ź�������
** �䡡��: NOT_OK:��������
**         OS_SEM_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OSSemIntPost,OSSched
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
** ��������: OSSemQuery
** ��������: ��ѯ�ź���
** �䡡��: Index:�ź�������
** �䡡��: �ź�����ֵ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
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