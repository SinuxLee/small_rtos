#include "os_cpu.h"
#include "os_cfg.h"

	NAME	OS_CPU_A_ASM

?PR?OSCtxSw?OS_CPU_A                     SEGMENT CODE ;INBLOCK 
?PR?OSIntCtxSw?OS_CPU_A                  SEGMENT CODE ;INBLOCK 
?PR?LoadCtx?OS_CPU_A                     SEGMENT CODE ;INBLOCK 
?PR?C_OSCtxSw?OS_CPU_C                   SEGMENT CODE 


	EXTRN	CODE (OSMapTbl)
	EXTRN	DATA (OSFastSwap)
	EXTRN	DATA (OSTaskID)
	EXTRN	DATA (OSNextTaskID)
	EXTRN	DATA (OSTsakStackBotton)
	EXTRN	DATA (Os_Enter_Sum)
IF EN_SP2  <> 0
	EXTRN	IDATA (Sp2)
ENDIF


	PUBLIC	LoadCtx
	PUBLIC	OSIntCtxSw
	PUBLIC	OSCtxSw
	PUBLIC	STACK 

;****************************************************************************************
;?STACK	SEGMENT IDATA

?STACK		SEGMENT   IDATA

		RSEG	?STACK
STACK:                                          ;堆栈
		DS	1

;****************************************************************************************
;/*********************************************************************************************************
;** 函数名称: LoadCtx
;** 功能描述: 任务环境恢复函数
;** 输　入: OSTaskID,OSFastSwap
;** 输　出 : 无
;** 全局变量: 无
;** 调用模块: 无
;********************************************************************************************************/

	RSEG  ?PR?LoadCtx?OS_CPU_A
LoadCtx:
	USING	0
	   
    POP     Os_Enter_Sum            ;恢复关中断计数器
                                    ;判断是否需要恢复所有寄存器
    MOV     A,OSTaskID
    CJNE    A,#OS_MAX_TASKS,LoadCtx_0
    SJMP    LoadCtx_2
LoadCtx_0:
    MOV     DPTR,#OSMapTbl
#if OS_MAX_TASKS < 9
    MOVC    A,@A+DPTR
    ANL     A,OSFastSwap
#else
    MOV     R6,OSFastSwap
    CLR     C
    SUBB    A,#8
    JNC     LoadCtx_1
    MOV     R6,OSFastSwap + 1
    MOV     A,OSTaskID
LoadCtx_1:
    MOVC    A,@A+DPTR
    ANL     A,R6
#endif
    JNZ     LoadCtx_2
                                    ;恢复寄存器
    POP     7
    POP     6
    POP     5
    POP     4
    POP     3
    POP     2
    POP     1
    POP     0
    POP     PSW
    POP     DPL
    POP     DPH
    POP     B
    POP     ACC
LoadCtx_2:
                                    ;判断是否需要开中断
    INC     Os_Enter_Sum
    djnz    Os_Enter_Sum,LoadCtx_3
    SET_EA                          ;开中断
LoadCtx_3:
    RET

;****************************************************************************************
;/*********************************************************************************************************
;** 函数名称: OSCtxSw
;** 功能描述: 任务主动放弃CPU环境保存函数
;** 输　入: OSTaskID
;** 输　出 : 无
;** 全局变量: OSFastSwap
;** 调用模块: 无
;********************************************************************************************************/
    RSEG  ?PR?OSCtxSw?OS_CPU_A
OSCtxSw:
    USING	0
                                    ;设置标志：任务再次恢复运行时不必恢复所有寄存器
    MOV     DPTR,#OSMapTbl
    MOV     A,OSTaskID
#if OS_MAX_TASKS < 9
    MOVC    A,@A+DPTR
    ORL     A,OSFastSwap
    MOV     OSFastSwap,A
#else
    CLR     C
    SUBB    A,#8
    JC      OSCtxSw_1
    MOVC    A,@A+DPTR
    ORL     A,OSFastSwap
    MOV     OSFastSwap,A
    LJMP    C_OSCtxSw
OSCtxSw_1:
    MOV     A,OSTaskID
    MOVC    A,@A+DPTR
    ORL     A,OSFastSwap+1
    MOV     OSFastSwap+1,A
#endif
    LJMP    C_OSCtxSw
;****************************************************************************************
;/*********************************************************************************************************
;** 函数名称: C_OSCtxSw
;** 功能描述: 堆栈处理函数
;** 输　入: 无
;** 输　出 : 无
;** 全局变量: OSTaskID,OSTsakStackBotton,SP
;** 调用模块: LoadCtx
;********************************************************************************************************/
	RSEG  ?PR?C_OSCtxSw?OS_CPU_C
C_OSCtxSw:
    PUSH    Os_Enter_Sum            ;保存关中断计数器
    mov     r2,sp
    
;     cp1 = (unsigned char idata *)SP +1;
    MOV     R0,SP

IF EN_SP2  <> 0
    mov     sp,#(Sp2-1)             ;堆栈指向临时空间，允许“软非屏蔽中断”
ENDIF

    INC     R0
;     temp = (unsigned char )OSTsakStackBotton[OSNextTaskID+1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSNextTaskID
    MOV     R1,A
    MOV     A,@R1
    MOV     R7,A
;     cp2 = OSTsakStackBotton[OSTaskID+1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSTaskID
    MOV     R1,A
    MOV     A,@R1
    MOV     R1,A
;     if( OSNextTaskID > OSTaskID)
    MOV     A,OSNextTaskID
    SETB    C
    SUBB    A,OSTaskID
    JC      ?C0001
;     {
;         while(cp2 != (unsigned char idata *)temp)
;         {
;             *cp1++ = *cp2++;
;         }
    MOV     A,R7
    CLR     C
    SUBB    A,R1
    MOV     R6,A
?C0002:
    MOV     A,@R1
    MOV     @R0,A
    INC     R0
    INC     R1
    DJNZ    R6,?C0002
?C0003:
;         temp = OSTsakStackBotton[OSTaskID+1] - (unsigned char idata *)SP-1;
    MOV     A,#LOW (OSTsakStackBotton+1)
    ADD     A,OSTaskID
    MOV     R1,A
    MOV     A,@R1
    SETB    C
    ;SUBB    A,sp
    SUBB    A,r2
    MOV     R7,A
;         SP = (unsigned char )cp1 - 1;
    DEC     R0;
    MOV     SP,R0
;         for(i = OSTaskID+1;i < OSNextTaskID+1; i++)
;         {
;             OSTsakStackBotton[i] -= temp;
;         }
    MOV     A,OSNextTaskID
    CLR     C
    SUBB    A,OSTaskID
    MOV     R6,A
    JZ      ?C0005

    MOV     A,#LOW (OSTsakStackBotton)
    ADD     A,OSTaskID
    MOV     R1,A    
    MOV     A,R7
    CPL     A
    INC     A
    MOV     R7,A
?C0004:
    INC     R1
    MOV     A,R7
    ADD     A,@R1    
    MOV     @R1,A
    DJNZ    R6,?C0004
?C0005:
;         OSTaskID = OSNextTaskID;
    MOV     OSTaskID,OSNextTaskID
;         LoadCtx();    
    LJMP    LoadCtx
;     }
?C0001:
; 
;     if( OSNextTaskID != OSTaskID)
    MOV     A,OSNextTaskID
    XRL     A,OSTaskID
    JZ      ?C000r
;     {
;          cp2--;
;          cp1--;
;         while(cp2 != (unsigned char idata *)temp)
;         {
;             *cp2-- = *cp1--;
;         }
    ;MOV     A,R7
    ;CLR     C
    ;SUBB    A,R1
    ;MOV     R6,A
    mov     a,r0
    clr     c
    subb    a,r7
    mov     r6,a
?C0008:
    DEC     R0
    DEC     R1
    MOV     A,@R0
    MOV     @R1,A
    DJNZ    R6,?C0008
?C0009:
;         temp = OSTsakStackBotton[OSTaskID+1] - (unsigned char idata *)SP-1;
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSTaskID
    MOV     R1,A
    MOV     A,@R1
    SETB    C
    ;SUBB    A,SP
    SUBB    A,r2
    MOV     R7,A
;         SP = (unsigned char )OSTsakStackBotton[OSNextTaskID+1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSNextTaskID
    MOV     R1,A
    MOV     A,@R1
    MOV     SP,A
;         for(i = OSNextTaskID+1;i < OSTaskID+1; i++)
;         {
;             OSTsakStackBotton[i] += temp;
;         }

    MOV     A,OSTaskID
    CLR     C
    SUBB    A,OSNextTaskID
    JZ      ?C0011

    MOV     R6,A
    MOV     A,#LOW (OSTsakStackBotton)
    ADD     A,OSNextTaskID
    MOV     R1,A    
?C0010:
    INC     R1
    MOV     A,R7
    ADD     A,@R1    
    MOV     @R1,A
    DJNZ    R6,?C0010

?C0011:
;         OSTaskID = OSNextTaskID;        
    MOV  	OSTaskID,OSNextTaskID
;         SP--;
    DEC  	SP
;     }
?C0007:
;     LoadCtx();
    LJMP 	LoadCtx
?C000r:
IF EN_SP2  <> 0
    mov     SP,r2
ENDIF
    LJMP 	LoadCtx
;****************************************************************************************
;/*********************************************************************************************************
;** 函数名称: OSIntCtxSw
;** 功能描述: 中断使任务放弃CPU环境保存函数
;** 输　入: OSTaskID
;** 输　出 : 无
;** 全局变量: OSFastSwap
;** 调用模块: 无
;********************************************************************************************************/
	RSEG  ?PR?OSIntCtxSw?OS_CPU_A
OSIntCtxSw:
	USING	0
                                        ;是否是优先级最低任务
    MOV     A,#OS_MAX_TASKS
    XRL     A,OSTaskID
    JNZ     OSIntCtxSw_0
                                        ;是则不需要保存所有寄存器
;SP=SP-13-4                             ;4:两层函数调用堆栈，13：寄存器数目
    MOV     A,#(-17)
    ADD     A,SP
    MOV     SP,A
                                        ;跳转到OSCtxSw，同时通知CPU中断处理完成
    MOV	    A, #LOW  OSCtxSw
    PUSH    ACC
    MOV	    A, #HIGH OSCtxSw
    PUSH    ACC
    RETI
                                        ;需要保存所有寄存器
OSIntCtxSw_0:
;SP=SP-4                                ;4:两层函数调用堆栈
    MOV     A,#0FCH
    ADD     A,SP
    MOV     SP,A
                                        ;设置标志：任务再次恢复运行时需要恢复所有寄存器
    MOV     DPTR,#OSMapTbl
    MOV     A,OSTaskID
#if OS_MAX_TASKS < 9
    MOVC    A,@A+DPTR
    CPL     A
    ANL     A,OSFastSwap
    MOV     OSFastSwap,A    
#else
    CLR     C
    SUBB    A,#8
    JC      OSIntCtxSw_1    
    MOVC    A,@A+DPTR
    CPL     A    
    ANL     A,OSFastSwap
    MOV     OSFastSwap,A 
    SJMP    OSIntCtxSw_2
OSIntCtxSw_1:
    MOV     A,OSTaskID
    MOVC    A,@A+DPTR
    CPL     A    
    ANL     A,OSFastSwap+1
    MOV     OSFastSwap+1,A 
OSIntCtxSw_2:
#endif
                                        ;跳转到堆栈处理，同时通知CPU中断处理完成
    MOV	    A, #LOW  C_OSCtxSw
    PUSH    ACC
    MOV	    A, #HIGH C_OSCtxSw
    PUSH    ACC
    RETI

;****************************************************************************************
	END
