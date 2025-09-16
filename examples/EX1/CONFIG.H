/********************************/
/*     "以下为系统配置"           */
/********************************/
#pragma REGPARMS
#include <reg52.h>
#include <intrins.h>
#include <absacc.h>
#define const code

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/********************************/
/*     "操作系统定义"            */
/********************************/
#include "os_cfg.h"
#include "os_cpu.h"
#include "../../src/core/os.h"
#include "../../src/core/os_q.h"
#include "../../src/core/os_sem.h"

/********************************/
/*     "以下为程序配置"           */
/********************************/

//任务定义
#ifdef IN_OS_CPU_C 
extern void TaskA(void);
extern void TaskB(void);
extern void TaskC(void);
void  (* const TaskFuction[OS_MAX_TASKS])(void)={TaskA,TaskB,TaskC};
//函数数组TaskFuction[]保存了各个任务初始PC指针,其按任务ID(既优先级次序)顺序保存

#endif
