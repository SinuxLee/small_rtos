#include "config.h"

void main(void)
{
	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
    OSStart();
}

void TaskA(void)
{
    while (1)
    {
        OSWait(K_TMO,5);
    } 
}

void TaskB(void)
{
    while (1)
    {
        OSWait(K_TMO,10);
    }
}

void TaskC(void)
{
    while (1)
    {
        OSWait(K_TMO,15);
    }
}
