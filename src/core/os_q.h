#ifndef NOT_OK
#define NOT_OK      255                                 /* ��������                                     */
#endif

#define OS_Q_FULL   8                                   /* ������                                       */
#define OS_Q_NOT_OK 4                                   /* ����Ϣ                                       */
#define OS_Q_TMO    2                                   /* ��ʱ��                                       */
#define OS_Q_OK     1                                   /* �����ɹ�                                     */

#ifndef IN_OS_Q
/*********************************************************************************************************
** ��������: OSQCreate
** ��������: ��ʼ����Ϣ����
** �䡡��: Buf:Ϊ���з���Ĵ洢�ռ��ַ
**         SizeOfBuf:Ϊ���з���Ĵ洢�ռ��С
** �䡡��: NOT_OK:��������
**         OS_Q_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
extern uint8 OSQCreate(uint8 OS_Q_MEM_SEL *Buf, uint8 SizeOfBuf);

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
extern uint8 OSQPend(uint8 data *Ret, uint8 OS_Q_MEM_SEL *Buf, uint8 Tick);

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
extern uint8 OSQAccept(uint8 data *Ret, uint8 OS_Q_MEM_SEL *Buf);

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
extern uint8 OSQPost(uint8 OS_Q_MEM_SEL *Buf, uint8 Data);

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
extern uint8 OSQPostFront(uint8 OS_Q_MEM_SEL *Buf, uint8 Data);

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
extern uint8 OSQIntPost(uint8 OS_Q_MEM_SEL *Buf, uint8 Data);

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
extern uint8 OSQIntPostFront(uint8 OS_Q_MEM_SEL *Buf, uint8 Data);

/*********************************************************************************************************
** ��������: OSQNMsgs
** ��������: ȡ����Ϣ��������Ϣ��
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
extern uint8 OSQNMsgs(uint8 OS_Q_MEM_SEL *Buf);

/*********************************************************************************************************
** ��������: OSQSize
** ��������: ȡ����Ϣ����������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ����������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
extern uint8 OSQSize(uint8 OS_Q_MEM_SEL *Buf);

/*********************************************************************************************************
** ��������: OSQFlush
** ��������: ��ն���
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
extern void OSQFlush (uint8 OS_Q_MEM_SEL *Buf);

#endif