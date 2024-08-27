#include "config.h"
#include "display.h"

void main(void)
{
	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); 
    PC_DispStr(15,  0, "Small RTOS(51), the Real-Time Kernel(For Keil c51)", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
    PC_DispStr(35,  1, "ChenMingJi", DISP_FGND_WHITE);
    PC_DispStr(35,  3, "EXAMPLE #3", DISP_FGND_WHITE);
	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;
    OSStart();
}

uint8 ShowChar;

 void TaskA(void)
{
    uint8 x,y;
    
    while (1)
    {
        OSWait(K_SIG,0);
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, ShowChar, DISP_FGND_LIGHT_GRAY);    
    }    
}


void TaskB(void)
{
    while (1)
    {
        ShowChar = OSRunningTaskID()+'0';
        OSSendSignal(0);
        OSWait(K_TMO,1);
    }    
}
