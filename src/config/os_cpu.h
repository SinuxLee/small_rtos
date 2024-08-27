#define EN_SP2        0                                         /* ��ֹ(0)������(1)���������ж�               */

#ifdef __C51__

typedef unsigned char  uint8;                                   /* �������ֲ���޷���8λ�����ؼ���            */
typedef signed   char  int8;                                    /* �������ֲ���з���8λ�����ؼ���            */
typedef unsigned int   uint16;                                  /* �������ֲ���޷���16λ�����ؼ���           */
typedef signed   int   int16;                                   /* �������ֲ���з���16λ�����ؼ���           */
typedef unsigned long  uint32;                                  /* �������ֲ���޷���32λ�����ؼ���           */
typedef signed   long  int32;                                   /* �������ֲ���з���32λ�����ؼ���           */

#ifndef NULL
#define NULL 0
#endif

#if EN_OS_INT_ENTER >0 
#define  OS_INT_ENTER() OSIntNesting++,EA=1
#endif

#define  OS_ENTER_CRITICAL()  EA = 0,Os_Enter_Sum++             /* ��ֹ�ж�                                     */
#define  OS_EXIT_CRITICAL()   if (--Os_Enter_Sum==0) EA = 1     /* �����ж�                                     */

#define  HIGH_BYTE  0                                           /* uint16�ĸ�λ�ֽ�                              */
#define  LOW_BYTE   1                                           /* uint16�ĵ�λ�ֽ�                              */

#define  OS_TASK_SW()         OSCtxSw()                         /* �����л�����                                  */


#define  IDATA_RAM_SIZE  0x100                                  /* idata��С                                    */

extern void OSCtxSw(void);
extern void OSIntCtxSw(void);
#ifndef IN_OS_CPU_C
extern void OSStart(void);
#endif

#define  Sp2Space       4                                       /* �߼��жϣ����������жϣ���ջ��С EN_SP2Ϊ0ʱ��Ч*/

#define OS_TIME_ISR     1                                       /* ϵͳ��ʱ��ʹ�õ��ж�                                       */

#endif



#ifndef __C51__

SET_EA   MACRO                                                  ;�����������ж�
             SETB     EA
         ENDM         
#endif