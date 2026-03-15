/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-4.  Get Serial Port Status Bytes
*/


/* Get serial port line and modem status */
void readSerialPortStatus( unsigned char whichPort,
                           unsigned char *lineStatus,
                           unsigned char *modemStatus )
{
union REGS regs;

regs.h.ah = 3;                 /* AH = function 3           */
regs.x.dx = whichPort;         /* DX = serial port 0,1,2,3  */
int86( 0x14, &regs, &regs );   /* BIOS Get Port Status      */

*lineStatus = regs.h.ah;       /* Returns AH = line status  */
*modemStatus = regs.h.al;      /*     and AL = modem status */
}
