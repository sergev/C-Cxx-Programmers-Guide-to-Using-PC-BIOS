/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-7.  Receive Character from Serial Port
*/


/* Read byte from serial port and return line status */
unsigned char rxSerialChar( unsigned int port,
                            unsigned char *byteRead )
{
union REGS regs;

regs.h.ah = 2;                     /* AH = function 2                   */
regs.x.dx = port;                  /* DX = serial port number (0,1,2,3) */
int86( 0x14, &regs, &regs );       /* Call BIOS                         */

*byteRead = regs.h.al;             /* AL = byte read                    */
return( regs.h.ah );               /* AH = line status                  */
}
