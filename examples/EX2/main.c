#include "config.h"
#include "display.h"

void main(void)
{
	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); 
    PC_DispStr(15,  0, "Small RTOS(51), the Real-Time Kernel(For Keil c51)", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
    PC_DispStr(35,  1, "ChenMingJi", DISP_FGND_WHITE);
    PC_DispStr(35,  3, "EXAMPLE #2", DISP_FGND_WHITE);
	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;
    OSStart();
}


void TaskA(void)
{
    uint8 x,y;
    
    while (1)
    {
        OS_ENTER_CRITICAL();
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, '1', DISP_FGND_LIGHT_GRAY);    
        OS_EXIT_CRITICAL();
        OSWait(K_TMO,1);
    }    
}

void TaskB(void)
{
    uint8 x,y;
    
    while (1)
    {
        OS_ENTER_CRITICAL();
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, '2', DISP_FGND_LIGHT_GRAY);    
        OS_EXIT_CRITICAL();
        OSWait(K_TMO,1);
    }    
}


void TaskC(void)
{
    uint8 x,y;
    
    while (1)
    {
        OS_ENTER_CRITICAL();
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, '3', DISP_FGND_LIGHT_GRAY);    
        OS_EXIT_CRITICAL();
        OSWait(K_TMO,1);
    }    
}


void TaskD(void)
{
    uint8 x,y;
    
    while (1)
    {
        OS_ENTER_CRITICAL();
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, '4', DISP_FGND_LIGHT_GRAY);    
        OS_EXIT_CRITICAL();
        OSWait(K_TMO,1);
    }    
}


void TaskE(void)
{
    uint8 x,y;
    
    while (1)
    {
        OS_ENTER_CRITICAL();
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, '5', DISP_FGND_LIGHT_GRAY);    
        OS_EXIT_CRITICAL();
        OSWait(K_TMO,1);
    }    
}
