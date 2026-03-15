/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-5.  Initialize a Serial Port
*/


/* Values for register AL - Break control */
#define BREAK_OFF   0
#define BREAK_ON    1

/* Values for register BH - parity control */
#define PARITY_NONE 0
#define PARITY_ODD  1
#define PARITY_EVEN 2
#define PARITY_BIT0 3
#define PARITY_BIT1 4

/* Values for register BL - stop bits per character */
#define STOPBITS_1  0
#define STOPBITS_2  1

/* Values for register CH - data bits per character */
#define DATABITS_5  0
#define DATABITS_6  1
#define DATABITS_7  2
#define DATABITS_8  3

/* Values for register CL - line speed */
#define BAUD_110    0
#define BAUD_150    1
#define BAUD_300    2
#define BAUD_600    3
#define BAUD_1200   4
#define BAUD_2400   5
#define BAUD_4800   6
#define BAUD_9600   7
#define BAUD_19200  8

void extInitSerialPort( unsigned char breakCode,
                        unsigned char parity,
                        unsigned char stopBits,
                        unsigned char dataBits,
                        unsigned char lineSpeed,
                        unsigned int whichPort,
                        unsigned char *lineStatus,
                        unsigned char *modemStatus )
{
union REGS regs;

regs.h.ah = 4;                  /* AH = function 4 - init port*/
regs.h.al = breakCode;          /* AL = Break control         */
regs.h.bh = parity;             /* BH = parity control        */
regs.h.bl = stopBits;           /* BL = number of stop bits   */
regs.h.ch = dataBits;           /* CH = number of data bits   */
regs.h.cl = lineSpeed;          /* CL = line speed            */
regs.x.dx = whichPort;          /* DX = port 0,1,2, or 3      */
int86( 0x14, &regs, &regs );    /* Call BIOS                  */

*lineStatus = regs.h.ah;        /* Returns line status in AH  */
*modemStatus = regs.h.al;       /* Returns modem status in AL */
}
