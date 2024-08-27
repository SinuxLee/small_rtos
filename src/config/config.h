/********************************/
/*     "����Ϊϵͳ����"           */
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
/*     "����ϵͳ����"            */
/********************************/
#include "os_cfg.h"
#include "os_cpu.h"
#include "../core/os.h"
#include "../core/os_q.h"
#include "../core/os_sem.h"

/********************************/
/*     "����Ϊ��������"           */
/********************************/

//������
#ifdef IN_OS_CPU_C 
extern void TaskA(void);
extern void TaskB(void);
extern void TaskC(void);
void  (* const TaskFuction[OS_MAX_TASKS])(void)={TaskA,TaskB,TaskC};
//��������TaskFuction[]�����˸��������ʼPCָ��,�䰴����ID(�����ȼ�����)˳�򱣴�

#endif
