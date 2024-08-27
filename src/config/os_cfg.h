#define OS_MAX_TASKS            3           /* ���������1~16                                           */
#define OS_TICKS_PER_SEC        10          /* ����1��ϵͳϵͳ������                                    */
#define EN_USER_TICK_TIMER      0           /* ��ֹ(0)������(1)ϵͳ��ʱ�жϵ����û�����UserTickTimer()  */
#define EN_OS_INT_ENTER         1           /* ��ֹ(0)������(1)�ж�Ƕ�׹���                             */
#define EN_TIMER_SHARING        1           /* ��ֹ(0)������(1)��ʱ���жϵ���OSTimeTick()               */
#define TICK_TIMER_SHARING      1           /* ���������ٴ�Ӳ���ж�Ϊһ��ϵͳ��ʱ�����ж�             */


#define EN_OS_Q                 0           /* ��ֹ(0)������(1)��Ϣ����                                 */
#define EN_OS_Q_CHK             0           /* ��ֹ(0)������(1)У����Ϣ����ָ��                         */
#define OS_Q_MEM_SEL            xdata       /* ��Ϣ���д洢�ռ�ѡ��keil c51���ã�����Ϊidata��xdata   */
                                            /* ����keil c51ʱ������Ϊ�ո�                               */
#define EN_OS_Q_PENT            1           /* ��ֹ(0)������(1)�ȴ������е���Ϣ                         */
#define EN_OS_Q_ACCEPT          0           /* ��ֹ(0)������(1)�޵ȴ��ĴӶ�����ȡ����Ϣ                 */
#define EN_OS_Q_POST            0           /* ��ֹ(0)������(1)FIFO��ʽ����з�����Ϣ                   */
#define EN_OS_Q_POST_FRONT      1           /* ��ֹ(0)������(1)LIFO��ʽ����з�����Ϣ                   */
#define EN_OS_Q_INT_POST        0           /* ��ֹ(0)������(1)�ж���FIFO��ʽ����з�����Ϣ             */
#define EN_OS_Q_INT_POST_FRONT  1           /* ��ֹ(0)������(1)�ж���LIFO��ʽ����з�����Ϣ             */
#define EN_OS_Q_NMsgs           1           /* ��ֹ(0)������(1)ȡ�ö�������Ϣ��                         */
#define EN_OS_Q_SIZE            0           /* ��ֹ(0)������(1)ȡ�ö���������                           */
#define EN_OS_Q_FLUSH           0           /* ��ֹ(0)������(1)��ն���                                 */

#define EN_OS_SEM               0           /* ��ֹ(0)������(1)�ź���                                   */
#define EN_OS_SEM_CHK           0           /* ��ֹ(0)������(1)У���ź�������                           */
#define OS_SEM_MEM_SEL          idata       /* �ź������ռ�ѡ��keil c51���ã�����Ϊidata��xdata       */
                                            /* ����keil c51ʱ������Ϊ�ո�                               */
#define OS_MAX_SEMS             2           /* ����ź�����Ŀ                                           */
#define EN_OS_SEM_PENT          1           /* ��ֹ(0)������(1)�ȴ��ź���                               */
#define EN_OS_SEM_ACCEPT        0           /* ��ֹ(0)������(1)�޵ȴ������ź���                         */
#define EN_OS_SEM_INT_POST      0           /* ��ֹ(0)������(1)�ж��з����ź���                         */
#define EN_OS_SEM_POST          1           /* ��ֹ(0)������(1)�з����ź���                             */
#define EN_OS_SEM_QUERY         0           /* ��ֹ(0)������(1)��ѯ�ź���                               */

//#define data                                /* ��keil c51ʱ�����ֹ��һ��                               */


#if EN_TIMER_SHARING == 0
#define TIME_ISR_TASK_ID     SHOW_TASK_ID   /* ����ϵͳ��ʱ�����жϴ���������ID                         */
#endif


#ifdef IN_OS_CPU_C 
#if EN_USER_TICK_TIMER > 0
extern void UserTickTimer(void);            /* ϵͳ��ʱ�ж��е��õ��û�����                                */
#endif
#endif