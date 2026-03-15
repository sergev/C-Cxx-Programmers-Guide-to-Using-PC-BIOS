/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-4.  Print Character
*/


unsigned char printChar( unsigned int whichPrinter, 
                         unsigned char chToPrint )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - print character */
regs.h.al = chToPrint;        /* AL = character */
regs.x.dx = whichPrinter;     /* DX = printer (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Call BIOS */

return( regs.h.ah );          /* AH = printer status */
}
