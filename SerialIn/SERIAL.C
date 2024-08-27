/*********************************************************************************************************
**				                               Small RTOS(51)
**                                   The Real-Time Kernel(For Keil c51)
**
**                                  (c) Copyright 2002-2003, chenmingji
**                                           All Rights Reserved
**
**                                                  V1.12.0
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: serial.c
**创   建   人: 陈明计
**最后修改日期: 2002年12月30日
**描        述: 为消息队列编制的测试代码。
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: V1.10
** 日　期: 2002年9月1日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.11.0
** 日　期: 2003年12月2日
** 描　述: 删除TaskC代码
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.12.0
** 日　期: 2003年12月30日
** 描　述: 删除TaskC代码
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"

uint8 Buf[NBYTE - 3];
uint8 OS_Q_MEM_SEL SerialData[14];


		void InitSerial(void)
{
	SCON = 0x50;
	PCON = 0x80;
	TMOD = TMOD & 0x0f;
	TMOD = TMOD | 0x20;
	TH1 = 0xf3;                   //com is 4800 b/s
	TL1 = 0xf3;
	TR1 = 1;
	ES = 1;

}


                            void main(void)
{
	InitSerial();
	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;
    OSStart();
}

        void comm(void) interrupt 4
{
    OS_INT_ENTER();
    if (RI == 1)
    {
        RI = 0;
#if EN_OS_Q_INT_POST > 0
        OSQIntPost(SerialData,SBUF);
#endif
#if EN_OS_Q_INT_POST_FRONT > 0
        OSQIntPostFront(SerialData,SBUF);
#endif

    }
    if (TI == 1)
    {
        TI = 0;
        OSIntSendSignal(1);
    }
    OSIntExit();
}

    void Recuve(void)
{
    uint8 data temp,temp1;
    uint8 Sum,i;

    OSQCreate(SerialData,14);

start:
#if EN_OS_Q_NMsgs > 0
    while (OSQNMsgs(SerialData) < NBYTE)
    {
        OSWait(K_TMO,1);
    }
#endif

    OSQPend(&temp,SerialData,0);
    while (1)
    {
        while (1)
        {
            if (OSQPend(&temp1,SerialData,20) == OS_Q_TMO)
            {
                goto start;
            }
            if ((temp == STARTBYTE1) && (temp1 == STARTBYTE2))
            {
                break;
            }            
            temp = temp1;
        }
        
start1:
        Sum = STARTBYTE1 + STARTBYTE2;
        
        if (OSQPend(&temp1,SerialData,20) == OS_Q_TMO)
        {
            goto start;
        }
        
        for (i = 0; i < NBYTE - 3; i++)
        {
            temp = temp1;
            if (OSQPend(&temp1,SerialData,20) == OS_Q_TMO)
            {
                goto start;
            }

            if ((temp == STARTBYTE1) && (temp1 == STARTBYTE2))
            {
                goto start1;
            }
            Buf[i] = temp;
            Sum += temp;
        }
        Sum += temp1;
        if (Sum == 0)
        {
            OSSendSignal(1);
        }        
    }
}


                            void TaskB(void)
{
    while (1)
    {
        OSWait(K_SIG,0);
        SBUF = 'O';
        OSWait(K_SIG,0);
        SBUF = 'K';
        OSWait(K_SIG,0);
        SBUF = '!';
        OSWait(K_SIG,0);
        SBUF = '\n';
        OSWait(K_SIG,0);
    } 
}

                            void TaskD(void)
{
    while (1)
    {
#if EN_OS_Q_POST > 0
        OSWait(K_TMO,10);
        OSQPost(SerialData,0xf0);
        OSWait(K_TMO,10);
        OSQPost(SerialData,0xf1);
        OSWait(K_TMO,10);
        OSQPost(SerialData,0xf0);
        OSWait(K_TMO,10);
        OSQPost(SerialData,0xf0);
        OSWait(K_TMO,10);
        OSQPost(SerialData,0x00);
        OSWait(K_TMO,10);
        OSQPost(SerialData,0x00);
        OSWait(K_TMO,10);
        OSQPost(SerialData,0x3f);
#endif
#if EN_OS_Q_POST_FRONT > 0
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0x3f);
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0x00);
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0x00);
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0xf0);
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0xf0);
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0xf1);
        OSWait(K_TMO,10);
        OSQPostFront(SerialData,0xf0);
#endif
        OSWait(K_TMO,200);
        
    } 
}

