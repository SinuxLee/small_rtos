#include "config.h"
#include "display.h"
#include <stdlib.h>

#define  DISP_MAX_X                     80       /* Maximum number of columns                          */
#define  DISP_MAX_Y                     25       /* Maximum number of rows                             */


                            uint8 random(uint8 seed)
{
	return ((((uint16) rand())^((uint16) rand())+TL0) % (uint16)seed);
}


                            void PC_DispClrScr (uint8 color)
{
    uint8 xdata *pscr;
    uint16 i;


    pscr = 0x0000;
    for (i = 0; i < (DISP_MAX_X * DISP_MAX_Y); i++)
    {
        *pscr++ = ' ';
        *pscr++ = color;
    }
}


                            void PC_DispChar (uint8 x,uint8 y,uint8 cc,uint8 color)
{
    uint8 xdata *pscr;


    pscr = (uint8 xdata *)((uint16)y * DISP_MAX_X * 2 + (uint16 )x * 2);
    *pscr++ = cc;
    *pscr = color;
}



                            void PC_DispStr (uint8 x,uint8 y,uint8 *s,uint8 color)
{
    uint8 xdata *pscr;

    pscr = (uint8 xdata *)((uint16 )y * DISP_MAX_X * 2 + (uint16)x * 2);
    while (*s) {
        *pscr++ = *s++;
        *pscr++ = color;
    }
}
