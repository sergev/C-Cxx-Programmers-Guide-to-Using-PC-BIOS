/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-2.  Get Printer Status 
*/


unsigned char getPrinterStatus( int whichPrinter )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - get printer status */
regs.x.dx = whichPrinter;     /* DX = printer (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}
