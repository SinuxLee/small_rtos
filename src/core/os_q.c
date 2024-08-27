#define  IN_OS_Q
#include "config.h"

#if EN_OS_Q > 0
/*********************************************************************************************************
** ��������: OSQCreate
** ��������: ��ʼ����Ϣ����
** �䡡��: Buf:Ϊ���з���Ĵ洢�ռ��ַ
**         SizeOfBuf:Ϊ���з���Ĵ洢�ռ��С
** �䡡��: NOT_OK:��������
**         OS_Q_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: ��
** Buf[0]:�������ֽ���,Buf[1]:Buf�ܳ���,Buf[2]:���Զ�,Buf[3](,Buf[4]):�ȴ����������б�
********************************************************************************************************/

uint8 OSQCreate(uint8 OS_Q_MEM_SEL *Buf, uint8 SizeOfBuf)
{
    OS_ENTER_CRITICAL();

    if ((SizeOfBuf >= 5) && (Buf != NULL))
    {
        Buf[0] = 0;                         /* ��������Ϣ��Ŀ           */
        Buf[1] = SizeOfBuf;                 /* ��Ϣ����ռ���ڴ��ֽ���    */
#if OS_MAX_TASKS < 9
        Buf[2] = 4;                         /* ��Ҫ���ӵ���Ϣ����λ��    */
#else
        Buf[2] = 5;                         /* ��Ҫ���ӵ���Ϣ����λ��    */
#endif
        Buf[3] = 0;                         /* ��Ϣ���еĵȴ������б�    */
        Buf[4] = 0;                         /* ���������ڵ���8ʱΪ�ȴ������б���һ���֣� */
                                            /* ����Ϊ��Ϣ������         */

        OS_EXIT_CRITICAL();
        return OS_Q_OK;
    }
    else
    {
        OS_EXIT_CRITICAL();
        return NOT_OK;
    }

}

/*********************************************************************************************************
** ��������: OSQPend
** ��������: �ȴ���Ϣ�����е���Ϣ
** �䡡��: Ret:���ص���Ϣ
**         Buf:ָ����е�ָ��
**         Tick:�ȴ�ʱ��
** �䡡��: NOT_OK:��������
**         OS_Q_OK:�յ���Ϣ
**         OS_Q_TMO:��ʱ��
**         OS_Q_NOT_OK:����Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OSRunningTaskID,OSClearSignal,OSSched,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_PENT > 0

uint8 OSQPend(uint8 data *Ret, uint8 OS_Q_MEM_SEL *Buf, uint8 Tick)
{
#ifdef __C51__
    uint8 data *cp;
#endif

#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return NOT_OK;
    }
#endif

    OS_ENTER_CRITICAL();

    OSWaitTick[OSRunningTaskID()] = Tick;       /* ���ó�ʱʱ��         */
                                                /* �����Ż��Ĵ�����ʹ��  */

                /* ʹ�ö�ջ��Ϊ��ʹ�������������� */
#ifdef __C51__
    SP++;
    *((uint8 data * data *)SP) = Ret;
#endif

                /* ���������ȴ�������� */
#if OS_MAX_TASKS < 9
    Buf[3] |= OSMapTbl[OSRunningTaskID()];
#else
    if (OSRunningTaskID() < 8)
    {
        Buf[3] |= OSMapTbl[OSRunningTaskID()];
    }
    else
    {
        Buf[4] |= OSMapTbl[OSRunningTaskID() & 0x07];
    }
#endif

    while (Buf[0] == 0)             /* ��Ϣ�������Ƿ�����Ϣ */
    {
                /* ����Ϣ */
#ifdef __C51__
        SP = SP + sizeof(Buf);
        *((uint8 OS_Q_MEM_SEL * data *)(SP + 1 - sizeof(Buf))) = Buf;
#endif

        OSClearSignal(OSRunningTaskID());   /* �������ȴ�״̬ */
        OSSched();                          /* ������һ������ */

#ifdef __C51__
        Buf = *((uint8 OS_Q_MEM_SEL * data *)(SP + 1 - sizeof(Buf)));
        SP = SP - sizeof(Buf);
#endif
                /* �����ٴ����У������ʱ�����˳�ѭ�� */
        if (OSWaitTick[OSRunningTaskID()] == 0)
        {
            break;
        }
    }

                /* ������ӵȴ����������������ɾ���� */
#if OS_MAX_TASKS < 9
    Buf[3] &= ~OSMapTbl[OSRunningTaskID()];
#else
    if (OSRunningTaskID() < 8)
    {
        Buf[3] &= ~OSMapTbl[OSRunningTaskID()];
    }
    else
    {
        Buf[4] &= ~OSMapTbl[OSRunningTaskID() & 0x07];
    }
#endif
                /* �ж���Ϣ�������Ƿ�����Ϣ */
    if (Buf[0] > 0)
    {
                /* �У���Ϣ���� */
        Buf[0]--;           /* ���е���Ϣ��Ŀ��һ */
                /* ָ����һ������λ�� */
        Buf[2]++;
        if (Buf[2] >= Buf[1] )
        {
#if OS_MAX_TASKS < 9
            Buf[2] = 4;
#else
            Buf[2] = 5;
#endif
        }
#ifdef __C51__
        cp = (uint8 data *)(*((uint8 data *)SP));
        SP--;
        *cp = Buf[Buf[2]];
#else
        *Ret = Buf[Buf[2]];
#endif
        OS_EXIT_CRITICAL();
        return OS_Q_OK;
    }
    else
    {
                /* �ޣ����ش����� */
#ifdef __C51__
        SP--;
#endif

        OS_EXIT_CRITICAL();
        return OS_Q_TMO;
    }
}
#endif

/*********************************************************************************************************
** ��������: OSQAccept
** ��������: �޵ȴ�����Ϣ������ȡ����Ϣ
** �䡡��: Ret:���ص���Ϣ
**         Buf:ָ����е�ָ��
** �䡡��: NOT_OK:��������
**         OS_Q_OK:�յ���Ϣ
**         OS_Q_TMO:��ʱ��
**         OS_Q_NOT_OK:����Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OSClearSignal,OSSched,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_ACCEPT > 0
uint8 OSQAccept(uint8 data *Ret, uint8 OS_Q_MEM_SEL *Buf)
{
#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return NOT_OK;
    }
#endif

    OS_ENTER_CRITICAL();

                /* �ж���Ϣ�������Ƿ�����Ϣ */
    if (*Buf != 0)
    {
                /* �У���Ϣ���� */
        Buf[0]--;
        Buf[2]++;
        if (Buf[2] >= Buf[1])
        {
#if OS_MAX_TASKS < 9
            Buf[2] = 4;
#else
            Buf[2] = 5;
#endif
        }
        *Ret = Buf[Buf[2]];
        OS_EXIT_CRITICAL();
        return OS_Q_OK;
    }
    else
    {
                /* �ޣ����ش����� */
        OS_EXIT_CRITICAL();
        return OS_Q_NOT_OK;
    }
}
#endif


/*********************************************************************************************************
** ��������: OSQIntPost
** ��������: �ж���FIFO��ʽ������Ϣ
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: OS_Q_FULL:������
**         OS_Q_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OSIntSendSignal,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_INT_POST > 0  || EN_OS_Q_POST > 0
uint8 OSQIntPost(uint8 OS_Q_MEM_SEL *Buf, uint8 Data)
{
    uint8 temp,i;

#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return NOT_OK;
    }
#endif

    OS_ENTER_CRITICAL();

                /* �����Ƿ����� */
#if OS_MAX_TASKS < 9
    if (Buf[0] < (Buf[1]-4))
#else
    if (Buf[0] < (Buf[1]-5))
#endif
    {
                /* ʹ�ö�ջ��Ϊ��ʹ�������������� */
#ifdef __C51__
        SP++;
        *((uint8 data *)SP) = Data;
#endif
                /* �����������Ϣ�洢λ�� */
        temp = Buf[2] + Buf[0] + 1;
        if (temp >= Buf[1])
        {
#if OS_MAX_TASKS < 9
            temp = temp - Buf[1]+4;
#else
            temp = temp - Buf[1]+5;
#endif
        }
                /* �洢��Ϣ */
#ifdef __C51__
        Buf[temp] = *((uint8 data *)SP);
        SP--;
#else
        Buf[temp] = Data;
#endif

        Buf[0]++;               /* ��Ϣ��Ŀ��һ */
                /* �ж���Ϣ���ǰ��Ϣ�����Ƿ�Ϊ�� */
        if (Buf[0] == 1)
        {
                /* Ϊ�գ�����������ȴ���Ϣ������֮ */
            temp = Buf[3];
#if OS_MAX_TASKS < 9
            for (i = 0; i < OS_MAX_TASKS; i++)
            {
                if ((temp & 0x01) != 0)
                {
                    break;
                }
                temp = temp >> 1;
            }
            if (i < OS_MAX_TASKS )
            {
                /* �鵽��ʹ�������ȼ���ߵ�����������״̬��������ӵȴ���������� */
                Buf[3] &= ~OSMapTbl[i];
                OSIntSendSignal(i);
            }
#else
            for (i = 0; i < 8; i++)
            {
                if ((temp & 0x01) != 0)
                {
                    break;
                }
                temp = temp >> 1;
            }
            if (i >= 8 )
            {
                temp = Buf[4];
                for (; i < OS_MAX_TASKS; i++)
                {
                    if ((temp & 0x01) != 0)
                    {
                        break;
                    }
                    temp = temp >> 1;
                }
            }
            if (i < OS_MAX_TASKS )
            {
                /* �鵽��ʹ�������ȼ���ߵ�����������״̬��������ӵȴ���������� */
                if (i < 8)
                {
                    Buf[3] &= ~OSMapTbl[i];
                }
                else
                {
                    Buf[4] &= ~OSMapTbl[i & 0x07];
                }
                OSIntSendSignal(i);
            }
#endif
        }
        OS_EXIT_CRITICAL();
        return OS_Q_OK;
    }
    else
    {
        OS_EXIT_CRITICAL();
        return OS_Q_FULL;
    }
}
#endif

/*********************************************************************************************************
** ��������: OSQIntPostFront
** ��������: �ж���LIFO��ʽ������Ϣ
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: OS_Q_FULL:������
**         OS_Q_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OSIntSendSignal,OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_INT_POST_FRONT > 0 || EN_OS_Q_POST_FRONT > 0
uint8 OSQIntPostFront(uint8 OS_Q_MEM_SEL *Buf, uint8 Data)
{
    uint8 temp,i;

#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return NOT_OK;
    }
#endif

    OS_ENTER_CRITICAL();

                /* �����Ƿ����� */
#if OS_MAX_TASKS < 9
    if (Buf[0] < (Buf[1]-4))
#else
    if (Buf[0] < (Buf[1]-5))
#endif
    {
                /* �洢��Ϣ */
        Buf[Buf[2]] = Data;
        Buf[0]++;           /* ��Ϣ��Ŀ����һ */
                /* ָ���µĽ�Ҫ���ӵ���Ϣ */
        Buf[2]--;
#if OS_MAX_TASKS < 9
        if (Buf[2] < 4)
#else
        if (Buf[2] < 5)
#endif
        {
            Buf[2] = Buf[1]-1;
        }
        
                /* �ж���Ϣ���ǰ��Ϣ�����Ƿ�Ϊ�� */
        if (Buf[0] == 1)
        {
                /* Ϊ�գ�����������ȴ���Ϣ������֮ */
            temp = Buf[3];
#if OS_MAX_TASKS < 9
            for (i = 0; i < OS_MAX_TASKS; i++)
            {
                if ((temp & 0x01) != 0)
                {
                    break;
                }
                temp = temp >> 1;
            }
            if (i < OS_MAX_TASKS )
            {
                /* �鵽��ʹ�������ȼ���ߵ�����������״̬��������ӵȴ���������� */
                Buf[3] &= ~OSMapTbl[i];
                OSIntSendSignal(i);
            }
#else
            for (i = 0; i < 8; i++)
            {
                if ((temp & 0x01) != 0)
                {
                    break;
                }
                temp = temp >> 1;
            }
            if (i >= 8 )
            {
                temp = Buf[4];
                for (; i < OS_MAX_TASKS; i++)
                {
                    if ((temp & 0x01) != 0)
                    {
                        break;
                    }
                    temp = temp >> 1;
                }
            }
            if (i < OS_MAX_TASKS )
            {
                /* �鵽��ʹ�������ȼ���ߵ�����������״̬��������ӵȴ���������� */
                if (i < 8)
                {
                    Buf[3] &= ~OSMapTbl[i];
                }
                else
                {
                    Buf[4] &= ~OSMapTbl[i & 0x07];
                }
                OSIntSendSignal(i);
            }
#endif
        }
        OS_EXIT_CRITICAL();
        return OS_Q_OK;
    }
    else
    {
        OS_EXIT_CRITICAL();
        return OS_Q_FULL;
    }
}
#endif

/*********************************************************************************************************
** ��������: OSQPost
** ��������: FIFO��ʽ������Ϣ
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: OS_Q_FULL:������
**         OS_Q_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OSQIntPost,OSSched
********************************************************************************************************/
#if EN_OS_Q_POST > 0
uint8 OSQPost(uint8 OS_Q_MEM_SEL *Buf, uint8 Data)
{
#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return NOT_OK;
    }
#endif

    if (OSQIntPost(Buf,Data) == OS_Q_OK)
    {
        OSSched();
        return OS_Q_OK;
    }
    else
    {
        return OS_Q_FULL;
    }
}
#endif

/*********************************************************************************************************
** ��������: OSQPostFront
** ��������: LIFO��ʽ������Ϣ
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: OS_Q_FULL:������
**         OS_Q_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OSQIntPostFront,OSSched
********************************************************************************************************/
#if EN_OS_Q_POST_FRONT > 0
uint8 OSQPostFront(uint8 OS_Q_MEM_SEL *Buf, uint8 Data)
{
#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return NOT_OK;
    }
#endif

    if (OSQIntPostFront(Buf,Data) == OS_Q_OK)
    {
        OSSched();
        return OS_Q_OK;
    }
    else
    {
        return OS_Q_FULL;
    }
}
#endif

/*********************************************************************************************************
** ��������: OSQNMsgs
** ��������: ȡ����Ϣ��������Ϣ��
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_NMsgs > 0
uint8 OSQNMsgs(uint8 OS_Q_MEM_SEL *Buf)
{
    uint8 temp;

#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return 0;
    }
#endif

    OS_ENTER_CRITICAL();
    temp = Buf[0];
    OS_EXIT_CRITICAL();
    return temp;
}
#endif

/*********************************************************************************************************
** ��������: OSQSize
** ��������: ȡ����Ϣ����������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ����������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_SIZE > 0
uint8 OSQSize(uint8 OS_Q_MEM_SEL *Buf)
{
    uint8 temp;

#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return 0;
    }
#endif

    OS_ENTER_CRITICAL();
#if OS_MAX_TASKS < 9
    temp = Buf[1] - 4;
#else
    temp = Buf[1] - 5;
#endif
    OS_EXIT_CRITICAL();
    return temp;
}
#endif

/*********************************************************************************************************
** ��������: OSQFlush
** ��������: ��ն���
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#if EN_OS_Q_FLUSH > 0
void OSQFlush (uint8 OS_Q_MEM_SEL *Buf)
{
#if EN_OS_Q_CHK > 0
    if (Buf == NULL)
    {
        return;
    }
#endif

    Buf[0] = 0;
#if OS_MAX_TASKS < 9
    Buf[2] = 4;
#else
    Buf[2] = 5;
#endif
}
#endif

#endif