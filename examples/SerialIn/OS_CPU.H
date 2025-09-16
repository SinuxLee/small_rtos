#define EN_SP2        0                                         /* 禁止(0)或允许(1)软非屏蔽中断               */

#ifdef __C51__

typedef unsigned char  uint8;                                   /* 定义可移植的无符号8位整数关键字            */
typedef signed   char  int8;                                    /* 定义可移植的有符号8位整数关键字            */
typedef unsigned int   uint16;                                  /* 定义可移植的无符号16位整数关键字           */
typedef signed   int   int16;                                   /* 定义可移植的有符号16位整数关键字           */
typedef unsigned long  uint32;                                  /* 定义可移植的无符号32位整数关键字           */
typedef signed   long  int32;                                   /* 定义可移植的有符号32位整数关键字           */


#ifndef NULL
#define NULL 0
#endif

#if EN_OS_INT_ENTER >0 
#define  OS_INT_ENTER() OSIntNesting++,EA=1
#endif

#define  OS_ENTER_CRITICAL()  EA = 0,Os_Enter_Sum++             /* 禁止中断                                     */
#define  OS_EXIT_CRITICAL()   if (--Os_Enter_Sum==0) EA = 1     /* 允许中断                                     */

#define  HIGH_BYTE  0                                           /* uint16的高位字节                              */
#define  LOW_BYTE   1                                           /* uint16的低位字节                              */

#define  OS_TASK_SW()         OSCtxSw()                         /* 任务切换函数                                  */


#define  IDATA_RAM_SIZE  0x100                                  /* idata大小                                    */

extern void OSCtxSw(void);
extern void OSIntCtxSw(void);
#ifndef IN_OS_CPU_C
extern void OSStart(void);
#endif

#define  Sp2Space       4                                       /* 高级中断（软非屏蔽中断）堆栈大小 EN_SP2为0时无效*/

#define OS_TIME_ISR     1                                       /* 系统定时器使用的中断                                       */

#endif



#ifndef __C51__

SET_EA   MACRO                                 ;打开所有允许中断
             SETB     EA
         ENDM
         
#endif
