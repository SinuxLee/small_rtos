#include "config.h"


void InitSerial(void)
{
	SCON = 0x50; // ���п��ƼĴ���
	PCON = 0x80; // ��Դ���ƼĴ���
	TMOD = TMOD & 0x0f; // ��ʱ��ģʽ�Ĵ���
	TMOD = TMOD | 0x20;
	TH1 = 0xf3;                   // com is 4800 b/s
	TL1 = 0xf3; // TH1 + TL1 timer1 ����ֵ
	TR1 = 1; // timer1 ����λ
	ES = 0; // �����жϿ���
}


void main(void)
{
	InitSerial();
	OSSemCreate(0,1);

    OSStart();
}

void putch(uint8 Data)
{
    uint8 i,j;

    SBUF = Data;
    i = 2048 / 256;
    j = 2048 % 256;
    do
    {
        do
        {
        } while (--j !=0);
    } while (--i != 0);
}


void TaskA(void)
{
    TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;
    EA = 1;
    while (1)
    {
        OSSemPend(0,0);
        putch('A');
        putch('0');
        putch('1');
        putch('2');
        putch('3');
        putch('\n');
        OSSemPost(0);
        OSWait(K_TMO,TL0);
    } 
}

void TaskB(void)
{
    while (1)
    {
        OSSemPend(0,0);
        putch('B');
        putch('0');
        putch('1');
        putch('2');
        putch('3');
        putch('\n');
        OSSemPost(0);
        OSWait(K_TMO,TL0);
    } 
}

void TaskC(void)
{
    while (1)
    {
        OSSemPend(0,0);
        putch('C');
        putch('0');
        putch('1');
        putch('2');
        putch('3');
        putch('\n');
        OSSemPost(0);
        OSWait(K_TMO,TL0);
    } 
}

void TaskD(void)
{
    uint8 i;
    
    while (1)
    {
        OSSemPend(0,0);
        for (i = 0; i < 100; i++)
        {
            putch('D');
            putch('E');
            putch('F');
            putch('G');
            putch(' ');
        }
        OSSemPost(0);
    } 
}
