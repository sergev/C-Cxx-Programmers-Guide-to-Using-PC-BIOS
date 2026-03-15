/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-6.  Transmit Character via Serial Port
*/


/* 
   Transmit character via serial
   port and return line status
*/
unsigned char txSerialChar( unsigned int port, 
                            unsigned char charToXmit )
{
union REGS regs;

regs.h.ah = 1;                     /* AH = function 1              */
regs.h.al = charToXmit;            /* AL = character to transmit   */
regs.x.dx = port;                  /* DX = port number (0,1,2,3)   */
int86( 0x14, &regs, &regs );       /* Call BIOS                    */
return( regs.h.ah );               /* AH = line status             */
}
