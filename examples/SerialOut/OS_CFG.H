#define OS_MAX_TASKS            4           /* 最大任务数1~16                                           */
#define OS_TICKS_PER_SEC        10          /* 声明1秒系统系统节拍数                                    */
#define EN_USER_TICK_TIMER      0           /* 禁止(0)或允许(1)系统定时中断调用用户函数UserTickTimer()  */
#define EN_OS_INT_ENTER         1           /* 禁止(0)或允许(1)中断嵌套管理                             */
#define EN_TIMER_SHARING        1           /* 禁止(0)或允许(1)定时器中断调用OSTimeTick()               */
#define TICK_TIMER_SHARING      1           /* 定义进入多少次硬件中断为一次系统定时器软中断             */

#define EN_OS_Q                 0           /* 禁止(0)或允许(1)消息队列                                 */
#define EN_OS_Q_CHK             0           /* 禁止(0)或允许(1)校验消息队列指针                         */
#define OS_Q_MEM_SEL            xdata       /* 消息队列存储空间选择，keil c51有用，必须为idata、xdata   */
                                            /* 不是keil c51时它必须为空格                               */
#define EN_OS_Q_PENT            1           /* 禁止(0)或允许(1)等待队列中的消息                         */
#define EN_OS_Q_ACCEPT          0           /* 禁止(0)或允许(1)无等待的从队列中取得消息                 */
#define EN_OS_Q_POST            0           /* 禁止(0)或允许(1)FIFO方式向对列发送消息                   */
#define EN_OS_Q_POST_FRONT      1           /* 禁止(0)或允许(1)LIFO方式向对列发送消息                   */
#define EN_OS_Q_INT_POST        0           /* 禁止(0)或允许(1)中断中FIFO方式相对列发送消息             */
#define EN_OS_Q_INT_POST_FRONT  1           /* 禁止(0)或允许(1)中断中LIFO方式相对列发送消息             */
#define EN_OS_Q_NMsgs           1           /* 禁止(0)或允许(1)取得队列中消息数                         */
#define EN_OS_Q_SIZE            0           /* 禁止(0)或允许(1)取得队列总容量                           */
#define EN_OS_Q_FLUSH           0           /* 禁止(0)或允许(1)清空队列                                 */

#define EN_OS_SEM               1           /* 禁止(0)或允许(1)信号量                                   */
#define EN_OS_SEM_CHK           0           /* 禁止(0)或允许(1)校验信号量索引                           */
#define OS_SEM_MEM_SEL          idata       /* 信号量储空间选择，keil c51有用，必须为idata、xdata       */
                                            /* 不是keil c51时它必须为空格                               */
#define OS_MAX_SEMS             2           /* 最大信号量数目                                           */
#define EN_OS_SEM_PENT          1           /* 禁止(0)或允许(1)等待信号量                               */
#define EN_OS_SEM_ACCEPT        0           /* 禁止(0)或允许(1)无等待请求信号量                         */
#define EN_OS_SEM_INT_POST      0           /* 禁止(0)或允许(1)中断中发送信号量                         */
#define EN_OS_SEM_POST          1           /* 禁止(0)或允许(1)中发送信号量                             */
#define EN_OS_SEM_QUERY         0           /* 禁止(0)或允许(1)查询信号量                               */

//#define data                                /* 非keil c51时必须加止这一句                               */


#if EN_TIMER_SHARING == 0
#define TIME_ISR_TASK_ID     SHOW_TASK_ID   /* 定义系统定时器软中断触发的任务ID                         */
#endif


#ifdef IN_OS_CPU_C 
#if EN_USER_TICK_TIMER > 0
extern void UserTickTimer(void);            /* 系统定时中断中调用的用户函数                                */
#endif
#endif
